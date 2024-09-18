// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyWidgetController.h"
#include "AbilitySystemComponent.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/PlayerState/BinggyPlayerState.h"

void UBinggyWidgetController::SetWidgetControllerParams(APlayerController* PC)
{
	PlayerController = PC;
	PlayerState = PC->GetPlayerState<ABinggyPlayerState>();
	// TODO: Potential Fix on the member variable
	ABinggyPlayerState* PS = Cast<ABinggyPlayerState>(PlayerState);
	AbilitySystemComponent = PS->GetAbilitySystemComponent();
	AttributeSet = PS->GetAttributeSet();
}

void UBinggyWidgetController::BroadcastInitialValue()
{
}

void UBinggyWidgetController::BindCallbacksToDependencies()
{
}

TObjectPtr<UBinggyAbilitySystemComponent> UBinggyWidgetController::GetBinggyAbilitySystemComponent() const
{
	return CastChecked<UBinggyAbilitySystemComponent>(AbilitySystemComponent);
}
