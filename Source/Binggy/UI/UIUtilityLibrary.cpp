// Fill out your copyright notice in the Description page of Project Settings.


#include "UIUtilityLibrary.h"

#include "Binggy/PlayerState/BinggyPlayerState.h"
#include "HUD/BinggyHUD.h"
#include "Kismet/GameplayStatics.h"

UOverlayWidgetController* UUIUtilityLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ABinggyHUD* HUD = Cast<ABinggyHUD>(PC->GetHUD()))
		{
			return HUD->GetOverlayWidgetController(PC);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UUIUtilityLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ABinggyHUD* HUD = Cast<ABinggyHUD>(PC->GetHUD()))
		{
			return HUD->GetAttributeMenuController(PC);
		}
	}
	return nullptr;
}
