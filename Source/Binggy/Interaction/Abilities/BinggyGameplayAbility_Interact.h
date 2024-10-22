// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Binggy/AbilitySystem/Abilities/BinggyGameplayAbility.h"
#include "BinggyGameplayAbility_Interact.generated.h"

struct FInteractionOption;
class UUserWidget;
/**
 *  Lyra used a indicator. 
 */
UCLASS()
class BINGGY_API UBinggyGameplayAbility_Interact : public UBinggyGameplayAbility
{
	GENERATED_BODY()

public:
	UBinggyGameplayAbility_Interact();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void UpdateInteractions(const TArray<FInteractionOption>& InteractiveOptions);

	UFUNCTION(BlueprintCallable)
	void TriggerInteraction();

protected:
	// Define 
	UPROPERTY(BlueprintReadWrite)
	TArray<FInteractionOption> CurrentOptions;

protected:
	UPROPERTY(EditDefaultsOnly)
	float InteractionScanRate = 0.1f;

	UPROPERTY(EditDefaultsOnly)
	float InteractionScanRange = 500;

	// UI widget
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> DefaultInteractionWidgetClass;
};
