// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyWidgetController.h"
#include "AbilitySystemComponent.h"
//#include "Binggy/AbilitySystem/BinggyAttributeSet.h"
//#include "GameFramework/PlayerController.h"

void UBinggyWidgetController::SetWidgetControllerParams(const FWidgetControllerParams WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UBinggyWidgetController::BroadcastInitialValue()
{
}

void UBinggyWidgetController::BindCallbacksToDependencies()
{
}
