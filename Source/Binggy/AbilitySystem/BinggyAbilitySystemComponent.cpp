// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyAbilitySystemComponent.h"

#include "Abilities/BinggyGameplayAbility.h"

void UBinggyAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UBinggyAbilitySystemComponent::AffectApplied);
}

void UBinggyAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UBinggyGameplayAbility* BinggyAbility = Cast<UBinggyGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(BinggyAbility->StartUpInputTag);
			GiveAbility(AbilitySpec);
		}
	}
	// Add ability is executed in PossessedBy, and HUD initialization is afterward
	// AbilityGivenDelegate.Broadcast();
}

void UBinggyAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UBinggyAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			// Continue to fire 
			TryActivateAbility(AbilitySpec.Handle);
			/*if (!AbilitySpec.IsActive()) */
		}
	}
}

void UBinggyAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// TODO: OtherWay
			CancelAbilityHandle(AbilitySpec.Handle);

			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UBinggyAbilitySystemComponent::AffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle)
{
	// GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, FString("Effect Applied"));
	FGameplayTagContainer AssetTags;
	
	EffectSpec.GetAllAssetTags(AssetTags);

	EffectAssetTags.Broadcast(AssetTags);
}


