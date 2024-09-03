// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeMenuWidgetController.h"


#include "Binggy/AbilitySystem/Attributes/BinggyAttributeSet.h"


void UAttributeMenuWidgetController::BroadcastInitialValue()
{
	Super::BroadcastInitialValue();
	UBinggyAttributeSet* AS = CastChecked<UBinggyAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	UBinggyAttributeSet* AS = CastChecked<UBinggyAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		}
	);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FBinggyAttributeInfo Info = AttributeInfoArray->GetAttributeInfoByTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
