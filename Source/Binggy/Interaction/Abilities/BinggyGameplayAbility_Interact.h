// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractionOption.h"
#include "AbilitySystem/BinggyAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/BinggyGameplayAbility.h"
#include "BinggyGameplayAbility_Interact.generated.h"

class UWidgetComponent;
class UUserWidget;
/**
 *  Lyra used a indicator, the class is a passive ability that scan the surrounding targets.
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

	// UI widget TODO: use TSoftClassPtr?
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> DefaultInteractionWidgetClass;

	TObjectPtr<UUserWidget> DebugInteractionWidgetInstance;

	// Debug
	TObjectPtr<UWidgetComponent> DebugWidgetComponent;
};
