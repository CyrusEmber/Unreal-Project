// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyGameplayAbility_Build.h"


#include "Actor/BinggyWorldBuildable.h"
#include "Components/SphereComponent.h"

void UBinggyGameplayAbility_Build::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo,
                                                   const FGameplayAbilityActivationInfo ActivationInfo,
                                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	InitializeAbility();
	// TODO Set it in here, when in battle, not allowed to activate
	CommitAbility(Handle, ActorInfo, ActivationInfo);
}

ABinggyWorldBuildable* UBinggyGameplayAbility_Build::SpawnBuildable(UStaticMesh* InBuildStaticMesh, FVector TargetLocation)
{
	// Only spawn the buildable in server
	if (!GetOwningActorFromActorInfo()->HasAuthority())
	{
		return nullptr;
	}
	if (!CurrentBuildable)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			// TODO the rotation should be facing the character first
			FRotator Rotation(0.0f, 0.0f, 0.0f); // Set the desired rotation

			CurrentBuildable = World->SpawnActor<ABinggyWorldBuildable>(BuildableClass, FVector::ZeroVector, Rotation);
			CurrentBuildable->OnConstructionBegin();
			CurrentBuildable->InitializeMeshAndOffset(InBuildStaticMesh);
		}
	}
	return CurrentBuildable;
}

void UBinggyGameplayAbility_Build::InitBuildable(ABinggyWorldBuildable* InBuildable)
{
	CurrentBuildable = InBuildable;

	CurrentBuildable->OnConstructionBegin();
}

void UBinggyGameplayAbility_Build::UpdateBuildMeshLocation(const FVector& TargetLocation, const FVector& HitNormal)
{
	// Only spawn the buildable in server
	if (!GetOwningActorFromActorInfo()->HasAuthority())
	{
		return;
	}

	if (CurrentBuildable)
	{
		CurrentBuildable->UpdatePreviewMeshPosition(TargetLocation, HitNormal, RotationAroundNormal);
	}
}

void UBinggyGameplayAbility_Build::UpdateMeshRotationAroundNormal(bool bIsRight)
{
	if (GetOwningActorFromActorInfo()->HasAuthority())
	{
		if (CurrentBuildable)
		{
			FRotator TargetRotation = RotationAroundNormal;
			TargetRotation = TargetRotation + (bIsRight ? -1 : 1) * DeltaRotation;
	
			RotationAroundNormal = TargetRotation;
		}
	} else
	{
		ServerUpdateMeshRotationAroundNormal(bIsRight);
	}

}

void UBinggyGameplayAbility_Build::ProcessCurrentHitResults(const FHitResult& HitResult)
{
	// The last HitResults is either a valid HitResult or fixed HitResult 
	UpdateBuildMeshLocation(HitResult.ImpactPoint, HitResult.Normal);
}

void UBinggyGameplayAbility_Build::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo,
                                              const FGameplayAbilityActivationInfo ActivationInfo,
                                              bool bReplicateEndAbility, bool bWasCancelled)
{
	if (CurrentBuildable)
	{
		if (GetOwningActorFromActorInfo()->HasAuthority())
		{
			CurrentBuildable->OnConstructionCompleted();
		}
	}
	// Reset the buildable
	CurrentBuildable = nullptr;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	// Destroy the cached actor
	// CurrentBuildStaticMeshActor->Destroy();
}

void UBinggyGameplayAbility_Build::InitializeAbility()
{
	RotationAroundNormal = FRotator(0, 0, 0);
}

FVector UBinggyGameplayAbility_Build::GetSymmetricPoint(FVector Point, FVector PlanePoint, FVector PlaneNormal)
{
	// Normalize the normal vector of the plane
	FVector NormalizedNormal = PlaneNormal.GetSafeNormal();

	// Vector from the point to the plane
	FVector V = Point - PlanePoint;

	// Project the vector onto the plane normal
	FVector VProj = FVector::DotProduct(V, NormalizedNormal) * NormalizedNormal;

	// Calculate the symmetric point
	FVector SymmetricPoint = Point - 2 * VProj;

	return SymmetricPoint;
}

void UBinggyGameplayAbility_Build::ServerUpdateMeshRotationAroundNormal_Implementation(bool bIsRight)
{
	UpdateMeshRotationAroundNormal(bIsRight);
}
