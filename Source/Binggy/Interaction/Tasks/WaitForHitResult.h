// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Interaction/InteractionQuery.h"
#include "WaitForHitResult.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitResultChangedEvent, const FHitResult&, HitResult);

/**
 * Should combine with WaitForInteratableTarget
 */
UCLASS()
class BINGGY_API UWaitForHitResult : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FHitResultChangedEvent HitResultChanged;

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UWaitForHitResult* WaitForHitResult(UGameplayAbility* OwningAbility, FCollisionProfileName TraceProfile, FInteractionQuery InInteractionQuery, TSubclassOf<UInterface> TargetInterfaceClass, float InteractionScanRange = 100, float InteractionScanRate = 0.100, bool bShowDebug = false);

protected:
	void AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, float MaxRange, FVector& OutTraceEnd, bool bIgnorePitch = false) const;

	// Perform line trace
	static void LineTrace(FHitResult& OutHitResult, const UWorld* World, const FVector& Start, const FVector& End, FName ProfileName, FCollisionQueryParams
	                      Params);
	
	// Clip the ray by ability range TODO: cannot use the function without static
	static bool ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter,
											float AbilityRange, FVector& ClippedPosition);

private:
	virtual void OnDestroy(bool AbilityEnded) override;
	
	void PerformTrace();
	
	UPROPERTY()
	FInteractionQuery InteractionQuery;
	
	FCollisionProfileName TraceProfile;

	TSubclassOf<UInterface> TargetInterfaceClass;

	float InteractionScanRange = 100;
	float InteractionScanRate = 0.100;
	bool bShowDebug = false;

	FTimerHandle TimerHandle;

	
};
