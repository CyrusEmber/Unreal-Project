// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "BinggyAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
