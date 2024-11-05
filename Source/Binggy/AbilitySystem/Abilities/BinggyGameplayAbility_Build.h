// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BinggyGameplayAbility.h"
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

	// Select the correct static mesh from UI
	UFUNCTION(BlueprintCallable, Category="Build")
	void UpdateBuildMesh(UStaticMesh* InBuildStaticMesh);

	// TODO a little problematic, how to get the target location correctly, or create a preview mesh
	UFUNCTION(BlueprintCallable, Category="Build")
	void UpdateBuildMeshLocation(FVector TargetLocation);

	UFUNCTION(BlueprintCallable, Category="Build")
	void RotateBuildMeshRotation(bool bIsPositive);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
private:
	TObjectPtr<ABinggyWorldBuildable> CurrentBuildable;

	FRotator DeltaRotation = FRotator(0, 20, 0);

	
};
