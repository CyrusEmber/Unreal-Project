// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeMenuWidgetController.h"


#include "AbilitySystemBlueprintLibrary.h"
#include "Binggy/AbilitySystem/BinggyGameplayTags.h"
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

void UAttributeMenuWidgetController::UnbindDelegateOnDestroy()
{
	AttributeInfoDelegate.Clear();
}

void UAttributeMenuWidgetController::UpdateAttribute(const FGameplayTag& AttributeTag)
{
	const FBinggyGameplayTags& GameplayTags = FBinggyGameplayTags::Get();
	
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;
	// AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AbilitySystemComponent->GetAvatarActor(), Payload.EventTag, Payload);
	
	// Handle attribute point change
	Payload.EventTag = GameplayTags.Attributes_Experience_AttributePoints;
	Payload.EventMagnitude = -1.f;
	// AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AbilitySystemComponent->GetAvatarActor(), Payload.EventTag, Payload);
}

void UAttributeMenuWidgetController::UnbindAllDelegates()
{
	UBinggyAttributeSet* AS = CastChecked<UBinggyAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).RemoveAll(this);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FBinggyAttributeInfo Info = AttributeInfoArray->GetAttributeInfoByTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
