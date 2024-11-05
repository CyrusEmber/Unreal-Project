// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderCursor.generated.h"

// Get the current mouse cursor location
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);
/**
 * 
 */
UCLASS()
class BINGGY_API UTargetDataUnderCursor : public UAbilityTask
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderCursor* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);
	void UpdateTargetData();

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;
private:

	virtual void Activate() override;
	void SendMouseCursorData();
	void TraceUnderCrosshairByVisibility(FHitResult& TraceHitResult);

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
	
private:
	float MeshLocationUpdateRate = 0.05;
	bool bShowDebug = false;

	FTimerHandle TimerHandle;
};
