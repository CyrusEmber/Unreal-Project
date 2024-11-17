// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitInputTriggered.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputTriggeredSignature, bool, Value);

struct FInputActionValue;
class UInputAction;
/**
 * Make sure that the input action is in the context mapping, local client task
 */
UCLASS()
class BINGGY_API UAbilityTask_WaitInputTriggered : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName = "WaitForInputTriggered", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAbilityTask_WaitInputTriggered* WaitInputTriggered(UGameplayAbility* OwningAbility, APlayerController* PC,
		UInputAction* InputAction, float Interval = 0.5f );

	// 
	UPROPERTY(BlueprintAssignable)
	FInputTriggeredSignature InputTriggered;

private:
	virtual void OnDestroy(bool AbilityEnded) override;
	virtual void Activate() override;

private:
	// Update the timer at below Interval
	float Interval;
	TWeakObjectPtr<APlayerController> PlayerController;
	TWeakObjectPtr<UInputAction> InputAction;
	FEnhancedInputActionEventBinding* BindingStart = nullptr;
	FEnhancedInputActionEventBinding* BindingEnd = nullptr;

	FTimerHandle TimerHandle;

	bool bRotateRight;

	void InputStartCallback(const FInputActionValue& Value);

	void InputTriggeredCallback(const FInputActionValue& Value);

	void InputCompleteCallback(const FInputActionValue& Value);

	void InputTrigger();

	
	
};
