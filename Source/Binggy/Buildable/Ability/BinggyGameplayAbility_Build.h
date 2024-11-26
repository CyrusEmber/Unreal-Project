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
	ABinggyWorldBuildable* SpawnBuildable(UStaticMesh* InBuildStaticMesh, FVector TargetLocation);

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
	void ProcessCurrentHitResults(const FHitResult& HitResult);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(BlueprintReadWrite, Category="Build")
	TObjectPtr<ABinggyWorldBuildable> CurrentBuildable;
	
private:
	

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

	// Helper function for snapping
	static FVector GetSymmetricPoint(FVector Point, FVector PlanePoint, FVector PlaneNormal);

	
};
