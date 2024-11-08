// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyWorldBuildable.h"

#include "Net/UnrealNetwork.h"

ABinggyWorldBuildable::ABinggyWorldBuildable()
{
	// TODO: Change this when the build is done.
	PrimaryActorTick.bCanEverTick = true;
	// Spawn on the server and replicate to the client
	bReplicates = true;
	
	SetMobility(EComponentMobility::Movable);
	// Ready for overlap
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
	GetStaticMeshComponent()->SetGenerateOverlapEvents(true);

	// SetMobility(EComponentMobility::Static);
	// GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetStaticMeshComponent()->SetIsReplicated(true);
	/*bReplicates = true;
	// SetReplicates(true);
	SetReplicatingMovement(true);*/
}

void ABinggyWorldBuildable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABinggyWorldBuildable, BuildableState);
}

void ABinggyWorldBuildable::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
                                                     FInteractionOptionBuilder& OptionBuilder)
{
	OptionBuilder.AddInteractionOption(Option);
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

void ABinggyWorldBuildable::OnConstructionCompleted()
{
	SetBuildableState(EBuildableState::Placed);
}

void ABinggyWorldBuildable::OnConstructionBegin()
{
	SetBuildableState(EBuildableState::Building);
}

void ABinggyWorldBuildable::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (BuildableState == EBuildableState::Placed)
	{
		return;
	}
	
#if WITH_EDITOR
	if (!HasAuthority()) // Server-only
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("NotifyActorBeginOverlap Execute in client! Bug!")); 
	}
#endif
	

	OverlappingActorCount++;
	SetBuildableState(EBuildableState::Blocked);
	
}

void ABinggyWorldBuildable::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (BuildableState == EBuildableState::Placed)
	{
		return;
	}

	OverlappingActorCount--;
	if (OverlappingActorCount == 0)
	{
		SetBuildableState(EBuildableState::Clear);
	}
	
}

void ABinggyWorldBuildable::OnRep_BuildableState(EBuildableState OldBuildableState)
{
	if (BuildableState == OldBuildableState)
	{
		return;
	}
	
	SetBuildableState(BuildableState);
}

void ABinggyWorldBuildable::SetBuildableState(EBuildableState NewBuildableState)
{
	BuildableState = NewBuildableState;
	switch (NewBuildableState)
	{
		case (EBuildableState::Building):
			SetMeshOverlayMaterial(GreenOverlayMaterial);
			GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
			break;

		case (EBuildableState::Clear):
			SetMeshOverlayMaterial(GreenOverlayMaterial);
			break;

		case (EBuildableState::Blocked):
			SetMeshOverlayMaterial(RedOverlayMaterial);
			break;

		// Should still generate overlap events
		case (EBuildableState::Placed):
			GetStaticMeshComponent()->SetOverlayMaterial(nullptr);
			GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			break;

		default:
			break;
	}
}


void ABinggyWorldBuildable::SetMeshOverlayMaterial(UMaterialInterface* InOverlayMaterial)
{
	GetStaticMeshComponent()->SetOverlayMaterial(InOverlayMaterial);
}
