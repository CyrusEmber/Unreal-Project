// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Binggy/AbilitySystem/Abilities/BinggyGameplayAbility.h"
#include "BinggyDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyDamageGameplayAbility : public UBinggyGameplayAbility
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Defaults")
    TSubclassOf<UGameplayEffect> DamageEffectClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
