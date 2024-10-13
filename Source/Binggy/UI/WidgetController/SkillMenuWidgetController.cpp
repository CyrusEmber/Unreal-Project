// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillMenuWidgetController.h"

#include "Binggy/UtilityLibrary.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/AbilitySystem/BinggyGameplayTags.h"
#include "Binggy/AbilitySystem/Attributes/BinggyAttributeSet.h"
#include "Binggy/AbilitySystem/Data/AbilityInfo.h"

void USkillMenuWidgetController::BroadcastInitialValue()
{
	// Super::BroadcastInitialValue();
	BroadcastAbilityInfoForAllAbilities();
}

void USkillMenuWidgetController::BindCallbacksToDependencies()
{
	// Super is empty
	Super::BindCallbacksToDependencies();
	AbilitySystemComponent->AbilityStatusChanged.AddUObject(this, &USkillMenuWidgetController::BroadcastAbilityInfo);
}

void USkillMenuWidgetController::BeginDestroy()
{
	Super::BeginDestroy();
	UnbindAllDelegates();
}

void USkillMenuWidgetController::BroadcastAbilityInfo(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag) const
{
	if (AbilityInfo)
	{
		FBinggyAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
		if (StatusTag.IsValid())
		{
			Info.StatusTag = StatusTag;
		}
		if (InputTag.IsValid())
		{
			Info.InputTag = InputTag;
		}
		AbilityInfoDelegate.Broadcast(Info);
	}
}

void USkillMenuWidgetController::EquipAbility(const FGameplayTag& AbilityToEquipAbilityTag, const FGameplayTag& EquippedAbilityInputTag)
{
	// AbilityToEquipAbilityTag is always valid if we set it in the skill menu
	// Get According Spec
	FGameplayAbilitySpec* AbilityToEquipSpec = AbilitySystemComponent->GetSpecFromAbilityTag(AbilityToEquipAbilityTag);
	// AbilityToEquip is empty, disable drag in UI or equip HERE
	if (!AbilityToEquipSpec)
	{
		return;
	}
	FGameplayAbilitySpec* EquippedAbilitySpec = AbilitySystemComponent->GetSpecsFromInputTag(EquippedAbilityInputTag);
	
	const FGameplayTag AbilityToEquipInputTag = UUtilityLibrary::GetInputTagFromSpec(*AbilityToEquipSpec);

	if (AbilityToEquipInputTag == EquippedAbilityInputTag)
	{
		return;
	}

	// The equipped ability is empty
	if (!EquippedAbilitySpec)
	{
		// Only change the according tag
		AbilityToEquipSpec->DynamicAbilityTags.RemoveTag(AbilityToEquipInputTag);
		AbilityToEquipSpec->DynamicAbilityTags.AddTag(EquippedAbilityInputTag);
		AbilitySystemComponent->AbilityStatusChanged.Broadcast(AbilityToEquipAbilityTag, FBinggyGameplayTags::Get().Ability_Status_Equipped, EquippedAbilityInputTag);
		// swapping between an equipped skill and an empty skill slot, broadcasting for the empty skill slot
		AbilitySystemComponent->AbilityStatusChanged.Broadcast(FGameplayTag(), FBinggyGameplayTags::Get().Ability_Status_Equipped, AbilityToEquipInputTag);
		// The AbilityToEquip is already equipped
		//MarkAbilitySpecDirty TODO?
		// AbilitySystemComponent->MarkAbilitySpecDirty(*AbilityToEquipSpec);
		return;
	}

	
	const FGameplayTag EquippedAbilityAbilityTag = UUtilityLibrary::GetAbilityTagFromSpec(*EquippedAbilitySpec);

	const FGameplayTag AbilityToEquipStatusTag = UUtilityLibrary::GetAbilityStatusTagFromSpec(*AbilityToEquipSpec);
	const FGameplayTag EquippedAbilityStatusTag = UUtilityLibrary::GetAbilityStatusTagFromSpec(*EquippedAbilitySpec);

	if (AbilityToEquipAbilityTag != EquippedAbilityAbilityTag)
	{
		// When ability tags are the same, the status tags are also the same
		AbilityToEquipSpec->DynamicAbilityTags.RemoveTag(AbilityToEquipStatusTag);
		AbilityToEquipSpec->DynamicAbilityTags.AddTag(EquippedAbilityStatusTag);

		EquippedAbilitySpec->DynamicAbilityTags.RemoveTag(EquippedAbilityStatusTag);
		EquippedAbilitySpec->DynamicAbilityTags.AddTag(AbilityToEquipStatusTag);
	}
	AbilityToEquipSpec->DynamicAbilityTags.RemoveTag(AbilityToEquipInputTag);
	AbilityToEquipSpec->DynamicAbilityTags.AddTag(EquippedAbilityInputTag);

	EquippedAbilitySpec->DynamicAbilityTags.RemoveTag(EquippedAbilityInputTag);
	EquippedAbilitySpec->DynamicAbilityTags.AddTag(AbilityToEquipInputTag);

	/*AbilitySystemComponent->MarkAbilitySpecDirty(*AbilityToEquipSpec);
	AbilitySystemComponent->MarkAbilitySpecDirty(*EquippedAbilitySpec);*/

	AbilitySystemComponent->AbilityStatusChanged.Broadcast(AbilityToEquipAbilityTag, EquippedAbilityStatusTag, EquippedAbilityInputTag);
	AbilitySystemComponent->AbilityStatusChanged.Broadcast(EquippedAbilityAbilityTag, AbilityToEquipStatusTag, AbilityToEquipInputTag);
	

	// Handle input tag and status tag
	/*if (AbilityToEquipTags.AbilityTag != EquippedAbilityTags.AbilityTag)
	{
		// When ability tags are the same, the status tags are also the same
		AbilityToEquipSpec->DynamicAbilityTags.RemoveTag(AbilityToEquipTags.StatusTag);
		AbilityToEquipSpec->DynamicAbilityTags.AddTag(EquippedAbilityTags.StatusTag);

		CurrentEquippedAbilitySpec->DynamicAbilityTags.RemoveTag(EquippedAbilityTags.StatusTag);
		CurrentEquippedAbilitySpec->DynamicAbilityTags.AddTag(AbilityToEquipTags.StatusTag);
	}
	AbilityToEquipSpec->DynamicAbilityTags.RemoveTag(AbilityToEquipTags.InputTag);
	AbilityToEquipSpec->DynamicAbilityTags.AddTag(EquippedAbilityTags.InputTag);
	
	CurrentEquippedAbilitySpec->DynamicAbilityTags.RemoveTag(EquippedAbilityTags.InputTag);
	CurrentEquippedAbilitySpec->DynamicAbilityTags.AddTag(AbilityToEquipTags.InputTag);

	AbilitySystemComponent->AbilityStatusChanged.Broadcast(AbilityToEquipTags.AbilityTag, EquippedAbilityTags.StatusTag, EquippedAbilityTags.InputTag);
	AbilitySystemComponent->AbilityStatusChanged.Broadcast(EquippedAbilityTags.AbilityTag, AbilityToEquipTags.StatusTag, AbilityToEquipTags.InputTag);*/
	

	// Find input tag for the ability tag, we might change the order of equipped abilities
	/*if (AbilityToEquipSpec && CurrentEquippedAbilitySpec)
	{
		for (const auto& Tag : AbilityToEquipSpec->DynamicAbilityTags)
		{
			// Swap the input tag
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Input"))))
			{
				OriginalInputTag  = Tag;
				AbilityToEquipSpec->DynamicAbilityTags.RemoveTag(Tag);
				CurrentEquippedAbilitySpec->DynamicAbilityTags.AddTag(Tag);
			}
		}
		// Set new status to equipped
		AbilitySystemComponent->SetStatusTagFromSpec(AbilityToEquipSpec, FBinggyGameplayTags::Get().Ability_Status_Equipped);
		
		// Set the input tag to the ability to equipped ability
		AbilityToEquipSpec->DynamicAbilityTags.AddTag(EquippedAbilityInputTag);
		// Set status to unlock if the original status is not equipped -> has input tag
		if (!OriginalInputTag.IsValid())
		{
			AbilitySystemComponent->SetStatusTagFromSpec(AbilityToEquipSpec, FBinggyGameplayTags::Get().Ability_Status_Unlocked);
		}
		// Broadcast TODO: the original ability could be equipped
		
	}*/
	
}

void USkillMenuWidgetController::UnbindAllDelegates()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityStatusChanged.RemoveAll(this);
	}
}
