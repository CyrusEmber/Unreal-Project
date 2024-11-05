// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyWorldBuildable.h"

ABinggyWorldBuildable::ABinggyWorldBuildable()
{
	// TODO: Change this when the build is done.
	bStaticMeshReplicateMovement = true;
	SetMobility(EComponentMobility::Movable);
	// Ready for overlap
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
	GetStaticMeshComponent()->SetGenerateOverlapEvents(true);

	bReplicates = true;
	// SetReplicates(true);
	SetReplicatingMovement(true);
}

void ABinggyWorldBuildable::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& OptionBuilder)
{
	OptionBuilder.AddInteractionOption(Option);
}

void ABinggyWorldBuildable::SetBuildMode()
{
	bStaticMeshReplicateMovement = false;
	SetMobility(EComponentMobility::Static);
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ABinggyWorldBuildable::SetBuildMesh(UStaticMesh* BuildMesh)
{
	GetStaticMeshComponent()->SetStaticMesh(BuildMesh);
}

void ABinggyWorldBuildable::UpdateMeshLocation(FVector TargetLocation)
{
	SetActorLocation(TargetLocation);
}

void ABinggyWorldBuildable::UpdateMeshRotation(FRotator TargetRotation)
{
	SetActorRotation(TargetRotation);
	
}

void ABinggyWorldBuildable::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}

void ABinggyWorldBuildable::BeginPlay()
{
	Super::BeginPlay();
    if (GetStaticMeshComponent())
    {
        GetStaticMeshComponent()->SetVisibility(true);         // Ensure visibility
        GetStaticMeshComponent()->SetHiddenInGame(false);      // Make sure it's not hidden
    }
	if (GetLocalRole() == ROLE_AutonomousProxy || GetLocalRole() == ROLE_SimulatedProxy)
	{
		// This is the client
		UE_LOG(LogTemp, Log, TEXT("Actor is now visible on the client!"));
	}
	else if (HasAuthority())
	{
		// This is the server
		UE_LOG(LogTemp, Log, TEXT("Actor is on the server."));
	}
}
