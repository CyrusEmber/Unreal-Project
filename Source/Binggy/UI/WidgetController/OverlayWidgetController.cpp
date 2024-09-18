// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"

#include "Binggy/UtilityLibrary.h"
#include "Binggy/AbilitySystem/Attributes/BinggyAttributeSet.h"
#include "Binggy/AbilitySystem/Data/AbilityInfo.h"


void UOverlayWidgetController::BroadcastInitialValue()
{
	// Super is empty
	Super::BroadcastInitialValue();
	//OnHealthChanged.Broadcast(AttributeSet->GetHealth());
	//OnMaxHealthChanged.Broadcast(AttributeSet->GetMaxHealth());

	// FIXME
	const UBinggyAttributeSet* BinggyAttributeSet = CastChecked<UBinggyAttributeSet>(AttributeSet);

}

// GetGameplayAttributeValueChangeDelegate this happens when certain attribute changes value
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// NO Super
	Super::BindCallbacksToDependencies();
	const UBinggyAttributeSet* BinggyAttributeSet = CastChecked<UBinggyAttributeSet>(AttributeSet);

	if (UBinggyAbilitySystemComponent* BinggyAbilitySystemComponent = GetBinggyAbilitySystemComponent())
	{
		// Add ability is executed in PossessedBy, and HUD initialization is afterward
		// BinggyAbilitySystemComponent->AbilityGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		OnInitializeStartupAbilities();
		check(MessageWidgetDataTable);
		// Add a callback with lambda
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
}

void UOverlayWidgetController::OnInitializeStartupAbilities() const
{
	BroadcastAbilityInfoForAllAbilities();
}

void UOverlayWidgetController::BroadcastAbilityInfoForAllAbilities() const
{
	// Lock the ability list to prevent changes while modifying it
	FScopedAbilityListLock ActiveScopeLock(*AbilitySystemComponent);
	for (const FGameplayAbilitySpec& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
	{
		FBinggyAbilityInfo Info = AbilityInfo->GetAbilityInfoByTag(UUtilityLibrary::GetAbilityTagFromSpec(AbilitySpec));
		if (Info.AbilityTag.IsValid())
		{
			Info.InputTag = UUtilityLibrary::GetInputTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	}
}
