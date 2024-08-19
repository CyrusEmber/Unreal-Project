// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyAbilitySystemComponent.h"

void UBinggyAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UBinggyAbilitySystemComponent::AffectApplied);
}

void UBinggyAbilitySystemComponent::AffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle)
{
	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, FString("Effect Applied"));
	FGameplayTagContainer AssetTags;
	
	EffectSpec.GetAllAssetTags(AssetTags);

	EffectAssetTags.Broadcast(AssetTags);


}
