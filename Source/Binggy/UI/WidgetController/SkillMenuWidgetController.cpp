// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillMenuWidgetController.h"

#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
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
	AbilitySystemComponent->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
	{
		if (AbilityInfo)
		{
			FBinggyAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
}

void USkillMenuWidgetController::UnbindAllDelegates()
{
	for (auto& Pair : AttributeSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).RemoveAll(this);
	}
}
