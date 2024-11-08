// Fill out your copyright notice in the Description page of Project Settings.


#include "Binggy/AbilitySystem/Abilities/BinggyGameplayAbility_Build.h"

#include "Binggy/Actor/BinggyWorldBuildable.h"

void UBinggyGameplayAbility_Build::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	// TODO Set it in here, when in battle, not allowed to activate
	CommitAbility(Handle, ActorInfo, ActivationInfo);
}

void UBinggyGameplayAbility_Build::SpawnBuildable(UStaticMesh* InBuildStaticMesh)
{
	// Only spawn the buildable in server
	if (!GetOwningActorFromActorInfo()->HasAuthority()) {
		return;
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
		}
	}
	UpdateBuildMesh(InBuildStaticMesh);
}

void UBinggyGameplayAbility_Build::UpdateBuildMesh(UStaticMesh* InBuildStaticMesh)
{
	if (CurrentBuildable)
	{
		CurrentBuildable->SetBuildMesh(InBuildStaticMesh);
	}
}

void UBinggyGameplayAbility_Build::UpdateBuildMeshLocation(FVector TargetLocation)
{
	// Only spawn the buildable in server
	if (!GetOwningActorFromActorInfo()->HasAuthority()) {
		return;
	}
	
	if (CurrentBuildable)
	{
		CurrentBuildable->SetActorLocation(TargetLocation);
	}
	
}

void UBinggyGameplayAbility_Build::RotateBuildMeshRotation(bool bIsPositive)
{
	if (CurrentBuildable)
	{
		FRotator NewRotation = CurrentBuildable->GetActorRotation() + (bIsPositive ? 1 : -1) * DeltaRotation;
		CurrentBuildable->SetActorRotation(NewRotation);
	}
}

void UBinggyGameplayAbility_Build::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              bool bReplicateEndAbility, bool bWasCancelled)
{
	if (CurrentBuildable)
	{
		if (GetOwningActorFromActorInfo()->HasAuthority()) {
			CurrentBuildable->OnConstructionCompleted();
		}
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	// Destroy the cached actor
	// CurrentBuildStaticMeshActor->Destroy();

	
}
