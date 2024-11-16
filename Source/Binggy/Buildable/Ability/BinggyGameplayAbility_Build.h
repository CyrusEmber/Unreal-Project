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
	
	// Select the correct static mesh from UI
	/*UFUNCTION(BlueprintCallable, Category="Build")
	void UpdateBuildMesh(UStaticMesh* InBuildStaticMesh);*/

	// Update the location based on its EBuildableSurfaceType and target location
	UFUNCTION(BlueprintCallable, Category="Build")
	void UpdateBuildMeshLocation(const FVector& TargetLocation, const FVector& HitNormal);

	UFUNCTION(BlueprintCallable, Category="Build")
	void RotateBuildMeshRotation(bool bIsPositive);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
private:
	TObjectPtr<ABinggyWorldBuildable> CurrentBuildable;

	FRotator DeltaRotation = FRotator(0, 20, 0);
	
	UPROPERTY(EditDefaultsOnly, Category="Binggy|Build", meta=(AllowPrivateAccess=true))
	TSubclassOf<ABinggyWorldBuildable> BuildableClass;

	
};
