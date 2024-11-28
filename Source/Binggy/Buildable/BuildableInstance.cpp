// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildable/BuildableInstance.h"

#include "Actor/BinggyWorldBuildable.h"
#include "Net/UnrealNetwork.h"

UBuildableInstance::UBuildableInstance()
{
}

void UBuildableInstance::SpawnBuildableActor(TSubclassOf<ABinggyWorldBuildable> BuildableClass,
	UStaticMesh* InBuildStaticMesh, TSet<EBuildableSurfaceType> AttachableSurfaces, FVector SpawnLocation)
{
	// Only spawn the buildable in server
	if (!Buildable)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			// TODO the rotation should be facing the character first
			FRotator Rotation(0.0f, 0.0f, 0.0f); // Set the desired rotation

			Buildable = World->SpawnActor<ABinggyWorldBuildable>(BuildableClass, FVector::ZeroVector, Rotation);
			Buildable->OnConstructionBegin();
			Buildable->SetAttachableSurfaces(AttachableSurfaces);
			Buildable->InitializeMesh(InBuildStaticMesh);
		}
	}
	
}

void UBuildableInstance::DestroyBuildableActor()
{
	if (Buildable)
	{
		Buildable->Destroy();
		Buildable = nullptr;
	}
}

void UBuildableInstance::SetBuildable(ABinggyWorldBuildable* InBuildable)
{
	Buildable = InBuildable;
}

void UBuildableInstance::OnRep_Owner()
{
}

void UBuildableInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UBuildableInstance, Owner);
}


