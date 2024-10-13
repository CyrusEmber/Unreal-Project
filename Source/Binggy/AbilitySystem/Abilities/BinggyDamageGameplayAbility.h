// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BinggyAbilityTypes.h"
#include "BinggyGameplayAbility.h"
#include "BinggyDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyDamageGameplayAbility : public UBinggyGameplayAbility
{
	GENERATED_BODY()

public:
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;
	
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
    TSubclassOf<UGameplayEffect> DamageEffectClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffChance = 20.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDamage = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffFrequency = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float ImpulseMagnitude = 60.f;


};
