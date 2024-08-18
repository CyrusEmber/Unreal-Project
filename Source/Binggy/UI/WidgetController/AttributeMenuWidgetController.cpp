// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeMenuWidgetController.h"

#include "Animation/AnimAttributes.h"
#include "Binggy/AbilitySystem/BinggyAttributeSet.h"
#include "Binggy/AbilitySystem/BinggyGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValue()
{
	Super::BroadcastInitialValue();
	UBinggyAttributeSet* AS = CastChecked<UBinggyAttributeSet>(AttributeSet);
	FBinggyAttributeInfo AttributeInfo = AttributeInfoArray->GetAttributeInfoByTag(
		FBinggyGameplayTags::Get().Attributes_Primary_Strength, true
		);
	AttributeInfo.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(AttributeInfo);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

}
