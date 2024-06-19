// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"
#include "Binggy/AbilitySystem/BinggyAttributeSet.h"



void UOverlayWidgetController::BroadcastInitialValue()
{
	// Super is empty
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
	const UBinggyAttributeSet* BinggyAttributeSet = CastChecked<UBinggyAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BinggyAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BinggyAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}
