// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"
#include "Binggy/AbilitySystem/Attributes/BinggyAttributeSet.h"




void UOverlayWidgetController::BroadcastInitialValue()
{
	// Super is empty
	Super::BroadcastInitialValue();
	//OnHealthChanged.Broadcast(AttributeSet->GetHealth());
	//OnMaxHealthChanged.Broadcast(AttributeSet->GetMaxHealth());

	// FIXME
	const UBinggyAttributeSet* BinggyAttributeSet = CastChecked<UBinggyAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(BinggyAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(BinggyAttributeSet->GetMaxHealth());

}

// GetGameplayAttributeValueChangeDelegate this happens when certain attribute changes value
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// NO Super
	Super::BindCallbacksToDependencies();
	const UBinggyAttributeSet* BinggyAttributeSet = CastChecked<UBinggyAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BinggyAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BinggyAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	check(MessageWidgetDataTable);
	// Add a callback with lambda
	Cast<UBinggyAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
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
