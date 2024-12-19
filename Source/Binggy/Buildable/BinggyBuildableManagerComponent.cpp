// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildable/BinggyBuildableManagerComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BuildableInstance.h"
#include "AbilitySystem/BinggyGameplayTags.h"
#include "Actor/BinggyWorldBuildable.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"


UBinggyBuildableManagerComponent::UBinggyBuildableManagerComponent()
{
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bCanEverTick = false;
}

void UBinggyBuildableManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UBinggyBuildableManagerComponent, CurrentBuildable, COND_OwnerOnly);
}

void UBinggyBuildableManagerComponent::CreateAndActivateBuildable(TSubclassOf<ABinggyWorldBuildable> BuildableClass)
{
	ServerCreateAndActivateBuildable(BuildableClass);
}

void UBinggyBuildableManagerComponent::ServerCreateAndActivateBuildable_Implementation(
	TSubclassOf<ABinggyWorldBuildable> BuildableClass)
{
	// TODO: store the result in the buildable manager
	UBuildableInstance* Result = nullptr;
	if (BuildableClass != nullptr)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FRotator Rotation(0.0f, 0.0f, 0.0f);
			// TODO
			CurrentBuildable = World->SpawnActor<ABinggyWorldBuildable>(BuildableClass,
				GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());

			Result = NewObject<UBuildableInstance>(GetOwner());
			Result->AddBuildable(CurrentBuildable);

			CurrentBuildable->OnConstructionBegin();
			// Server sent gameplay event
			if (Cast<ACharacter>(GetOwner())->GetController()->IsLocalController())
			{
				OnRep_CurrentBuildable();
			}
		}
	}
}

void UBinggyBuildableManagerComponent::ConnectTwoBuildables(ABinggyWorldBuildable* Parent,
	ABinggyWorldBuildable* Child)
{
	UBuildableInstance* Instance = Parent->GetBuildableInstance();
	Child->SetBuildableInstance(Instance);
	AttachTwoBuildables(Parent, Child);
}

void UBinggyBuildableManagerComponent::AttachTwoBuildables(ABinggyWorldBuildable* Parent, ABinggyWorldBuildable* Child)
{
	Child->GetStaticMeshComponent()->AttachToComponent(
	Parent->GetStaticMeshComponent(),
	FAttachmentTransformRules::SnapToTargetNotIncludingScale, // Attachment rule
	NAME_None  // Optional socket name
	);
}

void UBinggyBuildableManagerComponent::AddConstraintBetweenBuildables(ABinggyWorldBuildable* Parent, ABinggyWorldBuildable* Child)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Spawn the Physics Constraint Actor
	APhysicsConstraintActor* ConstraintActor = World->SpawnActor<APhysicsConstraintActor>(
		APhysicsConstraintActor::StaticClass(),
		FTransform(FQuat::Identity, FVector::ZeroVector, FVector::OneVector)
	);
	
	if (ConstraintActor)
	{
		ConstraintActor->SetOwner(GetOwner());
		UPhysicsConstraintComponent* ConstraintComponent = ConstraintActor->GetConstraintComp();

		// Set the constrained components
		ConstraintComponent->SetConstrainedComponents(
			Parent->GetStaticMeshComponent(),
			NAME_None,
			Child->GetStaticMeshComponent(),
			NAME_None
		);

		// Configure the constraint
		/*ConstraintComponent->SetLinearXLimit(LCM_Locked, 0.0f);
		ConstraintComponent->SetLinearYLimit(LCM_Locked, 0.0f);
		ConstraintComponent->SetLinearZLimit(LCM_Locked, 0.0f);*/

		ConstraintComponent->SetAngularSwing1Limit(ACM_Locked, 0.0f);
		ConstraintComponent->SetAngularSwing2Limit(ACM_Locked, 0.0f);
		ConstraintComponent->SetAngularTwistLimit(ACM_Locked, 0.0f);
		
		// Disable collision between constrained objects
		ConstraintComponent->SetDisableCollision(true);
	}
}

void UBinggyBuildableManagerComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UBinggyBuildableManagerComponent::OnRep_CurrentBuildable()
{
	if (CurrentBuildable != nullptr)
	{
		FGameplayEventData Payload;
		Payload.Target = CurrentBuildable;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(),
			FBinggyGameplayTags::Get().Ability_Build_Active, Payload);
	}
	CurrentBuildable = nullptr;
}


