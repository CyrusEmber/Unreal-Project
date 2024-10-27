// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Binggy/Interaction/InteractionOption.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/AbilitySystem/Abilities/BinggyGameplayAbility.h"
#include "BinggyGameplayAbility_Interact.generated.h"

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

protected:
	// Define 
	UPROPERTY(BlueprintReadWrite)
	TArray<FInteractionOption> CurrentOptions;

	// TODO: Seperate two classes?
	UFUNCTION(BlueprintCallable)
	void SetCurrentInteractionOptionsInASC(TArray<FInteractionOption> Options) { GetBinggyAbilitySystemFromActorInfo()->SetCurrentInteractionOptions(Options); }

	UFUNCTION(BlueprintCallable)
	TArray<FInteractionOption> GetCurrentInteractionOptionsInASC() { return GetBinggyAbilitySystemFromActorInfo()->GetCurrentInteractionOptions(); }

protected:
	UPROPERTY(EditDefaultsOnly)
	float InteractionScanRate = 0.1f;

	UPROPERTY(EditDefaultsOnly)
	float InteractionScanRange = 500;

	// UI widget
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> DefaultInteractionWidgetClass;
};