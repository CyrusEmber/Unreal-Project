// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"

#include "SkillMenuWidgetController.h"
#include "Binggy/AbilitySystem/Attributes/BinggyAttributeSet.h"
#include "Binggy/AbilitySystem/Data/AbilityInfo.h"


void UOverlayWidgetController::BroadcastInitialValue()
{
	// Super is empty
	Super::BroadcastInitialValue();
	BroadcastAbilityInfoForAllAbilities();
}

// GetGameplayAttributeValueChangeDelegate this happens when certain attribute changes value
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// NO Super
	Super::BindCallbacksToDependencies();

	if (UBinggyAbilitySystemComponent* BinggyAbilitySystemComponent = GetBinggyAbilitySystemComponent())
	{
		// Add ability is executed in PossessedBy, and HUD initialization is afterward
		check(MessageWidgetDataTable);
		
		// Add a callback with lambda, output message information
		BinggyAbilitySystemComponent->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
				{
					// TODO: change the location of tag
				
					for (const FGameplayTag& Tag : AssetTags)
					{
						FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag("Message");
						if (Tag.MatchesTag(MessageTag)) {
						
							const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
							//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Row->Message.ToString());
							MessageWidgetRowDelegate.Broadcast(*Row);
						}
					}
				}
		);
	}
	AbilitySystemComponent->AbilityStatusChanged.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
}

void UOverlayWidgetController::BroadcastAbilityInfo(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& InputTag) const
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

void UOverlayWidgetController::UnbindDelegates()
{
	MessageWidgetRowDelegate.RemoveAll(this);
	AbilitySystemComponent->AbilityStatusChanged.RemoveAll(this);
}


