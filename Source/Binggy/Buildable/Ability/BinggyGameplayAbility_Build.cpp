// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyGameplayAbility_Build.h"


#include "Actor/BinggyWorldBuildable.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

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

void UBinggyGameplayAbility_Build::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UBinggyGameplayAbility_Build, CurrentBuildable, COND_OwnerOnly);
}

ABinggyWorldBuildable* UBinggyGameplayAbility_Build::SpawnBuildable(UStaticMesh* InBuildStaticMesh, FVector TargetLocation)
{
	if (!CurrentBuildable)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			// TODO the rotation should be facing the character first
			FRotator Rotation(0.0f, 0.0f, 0.0f); // Set the desired rotation

			CurrentBuildable = World->SpawnActor<ABinggyWorldBuildable>(BuildableClass, FVector::ZeroVector, Rotation);
			CurrentBuildable->OnConstructionBegin();
			CurrentBuildable->InitializeMesh(InBuildStaticMesh);
		}
	}

	// Server update
	if (IsLocallyControlled())
	{
		OnRep_CurrentBuildable();
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
	UpdateBuildMeshLocation(HitResult.ImpactPoint, HitResult.Normal);
}

void UBinggyGameplayAbility_Build::OnSnappingBegin()
{
	CurrentBuildable->OnSnappingBegin();
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
	CurrentBuildable = nullptr;
	// Reset the buildable for instanced per actor ability
	HidePreview();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBinggyGameplayAbility_Build::HidePreview()
{
	CurrentBuildable = nullptr;
	// Server controlled logic
	if (IsLocallyControlled())
	{
		OnRep_CurrentBuildable();
	}
}

void UBinggyGameplayAbility_Build::OnRep_CurrentBuildable()
{
	LocalUpdatePreview(CurrentBuildable);
}


void UBinggyGameplayAbility_Build::InitializeAbility()
{
	RotationAroundNormal = FRotator(0, 0, 0);
}

void UBinggyGameplayAbility_Build::ServerUpdateMeshRotationAroundNormal_Implementation(bool bIsRight)
{
	UpdateMeshRotationAroundNormal(bIsRight);
}
