// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildable/BuildableInstance.h"

#include "Actor/BinggyWorldBuildable.h"
#include "Net/UnrealNetwork.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

UBuildableInstance::UBuildableInstance()
{
}

void UBuildableInstance::AddBuildable(ABinggyWorldBuildable* Buildable)
{
	if (Buildable)
	{
		Buildable->SetBuildableInstance(this);
		Buildables.Add(Buildable);
	}
}

void UBuildableInstance::RemoveBuildable(ABinggyWorldBuildable* Buildable)
{
	if (Buildable)
	{
		// TODO Set new buildable instance
	}
}


void UBuildableInstance::DestroyBuildableActor()
{
}

void UBuildableInstance::AddConstraintBetweenBuildables(ABinggyWorldBuildable* Buildable1,
	ABinggyWorldBuildable* Buildable2)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	FBuildablePair Pair(Buildable1, Buildable2);

	// The constraint exists
	if (Constraints.Contains(Pair))
	{
		return;
	}

	// Spawn the Physics Constraint Actor
	APhysicsConstraintActor* ConstraintActor = World->SpawnActor<APhysicsConstraintActor>();
	if (ConstraintActor)
	{
		UPhysicsConstraintComponent* ConstraintComponent = ConstraintActor->GetConstraintComp();

		// Set the constrained components
		ConstraintComponent->SetConstrainedComponents(
			Buildable1->GetStaticMeshComponent(),
			NAME_None,
			Buildable2->GetStaticMeshComponent(),
			NAME_None
		);

		// Configure the constraint
		ConstraintComponent->SetLinearXLimit(ELinearConstraintMotion::LCM_Limited, 0.0f);
		ConstraintComponent->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
		ConstraintComponent->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);

		ConstraintComponent->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
		ConstraintComponent->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
		ConstraintComponent->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0.0f);

		// Disable collision between constrained objects
		ConstraintComponent->SetDisableCollision(true);
	}
	// Add it to the TMap
	Constraints.Add(Pair, ConstraintActor);

}

void UBuildableInstance::OnRep_Owner()
{
}

void UBuildableInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UBuildableInstance, Owner);
}


