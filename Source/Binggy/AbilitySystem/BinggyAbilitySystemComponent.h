// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BinggyAbilitySystemComponent.generated.h" 

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);

/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Bind delegate
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;

protected:
	void AffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle);
	
};
