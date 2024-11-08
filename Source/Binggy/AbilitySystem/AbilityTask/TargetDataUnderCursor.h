// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderCursor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);
/**
 * Get the current mouse cursor location
 */
UCLASS()
class BINGGY_API UTargetDataUnderCursor : public UAbilityTask
{
	GENERATED_BODY()
public:
	// bIsLoop decide whether the task will loop it self with timer.
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderCursor* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility, bool bIsLoop);
	void UpdateTargetData();

	void BindCallbackToASC();

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;
private:
	virtual void OnDestroy(bool AbilityEnded) override;
	virtual void Activate() override;
	void SendMouseCursorData();

	// Get the hit result from camera and by visibility
	void TraceUnderCrosshairByVisibility(FHitResult& TraceHitResult);

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
	
private:
	float MeshLocationUpdateRate = 0.05;
	bool bShowDebug = false;
	bool bIsLoop = false;

	FTimerHandle TimerHandle;
};
