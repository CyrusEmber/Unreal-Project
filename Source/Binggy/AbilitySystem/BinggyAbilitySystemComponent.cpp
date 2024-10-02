// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BinggyGameplayTags.h"
#include "Abilities/BinggyGameplayAbility.h"
#include "Attributes/BinggyExperienceSet.h"
#include "Binggy/UtilityLibrary.h"
#include "Data/AbilityInfo.h"

struct FBinggyAbilityInfo;

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
			AbilitySpec.DynamicAbilityTags.AddTag(FBinggyGameplayTags::Get().Ability_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	// Add ability is executed in PossessedBy, and HUD initialization is afterward
	// AbilityGivenDelegate.Broadcast();
	this->GetGameplayAttributeValueChangeDelegate(UBinggyExperienceSet::GetLevelAttribute()).AddUObject(this, &UBinggyAbilitySystemComponent::UpdateAbilityStatus);
}

void UBinggyAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

// TODO find some solutions to reduce time complexity
FGameplayTag UBinggyAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UBinggyAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void UBinggyAbilitySystemComponent::UpdateAbilityStatus(const FOnAttributeChangeData& Data)
{
	// TODO turn this to experience component
	if (Data.OldValue == Data.NewValue)
	{
		return;
	}
	UAbilityInfo* AbilityInfo = UUtilityLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FBinggyAbilityInfo& Info : AbilityInfo->AbilitiesInformation)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Data.NewValue < Info.LevelRequirement) continue;
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(FBinggyGameplayTags::Get().Ability_Status_Unlocked);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(Info.AbilityTag, FBinggyGameplayTags::Get().Ability_Status_Unlocked);
		}
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

void UBinggyAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag);
}


