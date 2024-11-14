// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BinggyAbilitySystemGlobals.h"

#include "FBinggyGameplayEffectContext.h"

FGameplayEffectContext* UBinggyAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FBinggyGameplayEffectContext();
}
