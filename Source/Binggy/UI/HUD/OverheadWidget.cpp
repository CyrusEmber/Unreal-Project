// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverHeadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText) {
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverHeadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	ENetRole LocalRole = InPawn->GetLocalRole();
	FString Role;
	switch (LocalRole) {
	case ENetRole::ROLE_Authority:
		Role = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		Role = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		Role = FString("Simulated Proxy");
		break;
	case ENetRole::ROLE_None:
		Role = FString("None");
		break;
	case ENetRole::ROLE_MAX:
		Role = FString("Max");
		break;
	}
	FString RoleString = FString::Printf(TEXT("Local Role: %s"), *Role);
	SetDisplayText(RoleString);
}

void UOverHeadWidget::ShowPlayerName(APawn* InPawn)
{
	APlayerState* PlayerState = InPawn->GetPlayerState<APlayerState>();
	FString PlayerName;
	if (PlayerState) {
		PlayerName = PlayerState->GetPlayerName();
	}
	SetDisplayText(PlayerName);
}

void UOverHeadWidget::NativeDestruct()
{
	RemoveFromParent();
}

FText UOverHeadWidget::GetDisplayText()
{
	return DisplayText->GetText();
}
