// Fill out your copyright notice in the Description page of Project Settings.


#include "Binggy/AbilitySystem/Abilities/BinggyGameplayAbility_Build.h"

#include "Binggy/Actor/BinggyWorldBuildable.h"
#include "Binggy/BinggyLogChannel.h"

void UBinggyGameplayAbility_Build::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	// TODO Set it in here, when in battle, not allowed to activate
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	if (!HasAuthority(&ActivationInfo)) {
		UE_LOG(LogBinggy, Error, TEXT("Client activation"));
	}
}

void UBinggyGameplayAbility_Build::UpdateBuildMesh(UStaticMesh* InBuildStaticMesh)
{

	if (!CurrentBuildable)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			// TODO the rotation should be facing the character first
			FRotator Rotation(0.0f, 0.0f, 0.0f); // Set the desired rotation

			CurrentBuildable = World->SpawnActor<ABinggyWorldBuildable>(ABinggyWorldBuildable::StaticClass(), FVector::ZeroVector, Rotation);

			if (!CurrentBuildable)
			{
				UE_LOG(LogBinggy, Error, TEXT("CurrentBuildable not spawned"));
			}
		}

	}
	if (CurrentBuildable)
	{
		CurrentBuildable->SetBuildMesh(InBuildStaticMesh);
	}
	
}

void UBinggyGameplayAbility_Build::UpdateBuildMeshLocation(FVector TargetLocation)
{
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
		CurrentBuildable->SetBuildMode();
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	// Destroy the cached actor
	// CurrentBuildStaticMeshActor->Destroy();

	
}
