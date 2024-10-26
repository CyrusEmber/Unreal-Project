// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/AbilitySystem/Abilities/BinggyGameplayAbility.h"
#include "Binggy/Interaction/InteractionOption.h"
#include "Ability_TriggerInteract.generated.h"

/**
 * 
 */
UCLASS()
class BINGGY_API UAbility_TriggerInteract : public UBinggyGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void TriggerInteraction(TArray<FInteractionOption> CurrentOptions);

	UFUNCTION(BlueprintCallable)
	TArray<FInteractionOption> GetCurrentInteractionOptionsInASC() { return GetBinggyAbilitySystemFromActorInfo()->GetCurrentInteractionOptions(); }
};
