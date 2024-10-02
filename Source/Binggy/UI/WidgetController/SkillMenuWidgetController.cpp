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
	AbilitySystemComponent->AbilityStatusChanged.AddUObject(this, &USkillMenuWidgetController::BroadcastAbilityInfo);
}

void USkillMenuWidgetController::BeginDestroy()
{
	Super::BeginDestroy();
	UnbindAllDelegates();
}

void USkillMenuWidgetController::BroadcastAbilityInfo(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag) const
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Broadcast AbilityStatusChanged")); 
	if (AbilityInfo)
	{
		FBinggyAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
		Info.StatusTag = StatusTag;
		AbilityInfoDelegate.Broadcast(Info);
	}
}

void USkillMenuWidgetController::UnbindAllDelegates()
{
	check(AbilitySystemComponent);
	
	AbilitySystemComponent->AbilityStatusChanged.RemoveAll(this);	
}
