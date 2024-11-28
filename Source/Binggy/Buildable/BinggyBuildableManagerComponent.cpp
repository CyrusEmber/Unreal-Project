// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildable/BinggyBuildableManagerComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BuildableInstance.h"
#include "AbilitySystem/BinggyGameplayTags.h"
#include "Actor/BinggyWorldBuildable.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


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

void UBinggyBuildableManagerComponent::CreateAndActivateBuildable(TSubclassOf<ABinggyWorldBuildable> BuildableClass, AActor* PlayerCharacter)
{
	ServerCreateAndActivateBuildable(BuildableClass, PlayerCharacter);
}

void UBinggyBuildableManagerComponent::ServerCreateAndActivateBuildable_Implementation(
	TSubclassOf<ABinggyWorldBuildable> BuildableClass, AActor* PlayerCharacter)
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
			Result->SetBuildable(CurrentBuildable);

			CurrentBuildable->OnConstructionBegin();
			// Server sent gameplay event
			if (Cast<ACharacter>(GetOwner())->GetController()->IsLocalController())
			{
				OnRep_CurrentBuildable();
			}
		}
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


