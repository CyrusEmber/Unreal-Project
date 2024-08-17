// Fill out your copyright notice in the Description page of Project Settings.


#include "UIUtilityLibrary.h"

#include "Binggy/PlayerState/BinggyPlayerState.h"
#include "HUD/BinggyHUD.h"
#include "Kismet/GameplayStatics.h"

UOverlayWidgetController* UUIUtilityLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	// TODO: Fix the repetition, just pass in the PC?
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ABinggyHUD* HUD = Cast<ABinggyHUD>(PC->GetHUD()))
		{
			ABinggyPlayerState* PS = PC->GetPlayerState<ABinggyPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetParams = FWidgetControllerParams(PC, PS, ASC, AS);
			return HUD->GetOverlayWidgetController(WidgetParams);
		}
	}
	return nullptr;
}
