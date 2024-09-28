// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"

#include "Binggy/AbilitySystem/Attributes/BinggyAttributeSet.h"


void UOverlayWidgetController::BroadcastInitialValue()
{
	// Super is empty
	Super::BroadcastInitialValue();

}

// GetGameplayAttributeValueChangeDelegate this happens when certain attribute changes value
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// NO Super
	Super::BindCallbacksToDependencies();

	if (UBinggyAbilitySystemComponent* BinggyAbilitySystemComponent = GetBinggyAbilitySystemComponent())
	{
		// Add ability is executed in PossessedBy, and HUD initialization is afterward
		BroadcastAbilityInfoForAllAbilities();
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
}


