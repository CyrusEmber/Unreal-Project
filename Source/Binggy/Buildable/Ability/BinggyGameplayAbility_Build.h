// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BinggyGameplayAbility.h"
#include "BinggyGameplayAbility_Build.generated.h"

class ABinggyWorldBuildable;
/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyGameplayAbility_Build : public UBinggyGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// TODO: separate spawn and update
	UFUNCTION(BlueprintCallable, Category="Build")
	void SpawnBuildable(UStaticMesh* InBuildStaticMesh, FVector TargetLocation, FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category="Build")
	void InitBuildable(ABinggyWorldBuildable* InBuildable);
	
	// Select the correct static mesh from UI
	/*UFUNCTION(BlueprintCallable, Category="Build")
	void UpdateBuildMesh(UStaticMesh* InBuildStaticMesh);*/

	// Update the location based on its EBuildableSurfaceType and target location
	UFUNCTION(BlueprintCallable, Category="Build")
	void UpdateBuildMeshLocation(const FVector& TargetLocation, const FVector& HitNormal);

	// Value only being 1 or -1
	UFUNCTION(BlueprintCallable, Category="Build")
	void UpdateMeshRotationAroundNormal(bool bIsRight);
	
	UFUNCTION(BlueprintCallable, Category="Build")
	void PerformSphereOverlap(const FVector& TargetLocation, float Radius);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
private:
	TObjectPtr<ABinggyWorldBuildable> CurrentBuildable;

	FRotator DeltaRotation = FRotator(0, 15, 0);

	float LerpSpeed = 120.f;

	FRotator RotationAroundNormal = FRotator(0, 0, 0);
	
	UPROPERTY(EditDefaultsOnly, Category="Binggy|Build", meta=(AllowPrivateAccess=true))
	TSubclassOf<ABinggyWorldBuildable> BuildableClass;

	// Initialize for the Instanced per Actor ability
	void InitializeAbility();

	// Not necessarily have network authority, the owning actor
	UFUNCTION(Server, Reliable)
	void ServerUpdateMeshRotationAroundNormal(bool bIsRight);

	
};
