// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyPrimaryGameLayout.h"

#include "CommonUIExtensions.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "Components/Widget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BinggyPrimaryGameLayout)

UBinggyPrimaryGameLayout* UBinggyPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(
	const UObject* WorldContextObject)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	APlayerController* PlayerController = GameInstance->GetPrimaryPlayerController(false);
	return GetPrimaryGameLayout(PlayerController);
}

UBinggyPrimaryGameLayout* UBinggyPrimaryGameLayout::GetPrimaryGameLayout(APlayerController* PlayerController)
{
	// TODO CommonLocalPlayer?
	return PlayerController ? GetPrimaryGameLayout(Cast<ULocalPlayer>(PlayerController->Player)) : nullptr;
}

UBinggyPrimaryGameLayout* UBinggyPrimaryGameLayout::GetPrimaryGameLayout(ULocalPlayer* LocalPlayer)
{
	// TODO How to get the layout from policy????

	unimplemented();
	return nullptr;
}

UBinggyPrimaryGameLayout::UBinggyPrimaryGameLayout(const FObjectInitializer& ObjectInitializer)
{
}

void UBinggyPrimaryGameLayout::FindAndRemoveWidgetFromLayer(UCommonActivatableWidget* ActivatableWidget)
{
	// We're not sure what layer the widget is on so go searching.
	for (const auto& LayerKVP : Layers)
	{
		LayerKVP.Value->RemoveWidget(*ActivatableWidget);
	}
}

UCommonActivatableWidgetContainerBase* UBinggyPrimaryGameLayout::GetLayerWidget(FGameplayTag LayerName)
{
	return Layers.FindRef(LayerName);
}

void UBinggyPrimaryGameLayout::RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget)
{
	if (!IsDesignTime())
	{
		LayerWidget->OnTransitioningChanged.AddUObject(this, &UBinggyPrimaryGameLayout::OnWidgetStackTransitioning);
		// TODO: Consider allowing a transition duration, we currently set it to 0, because if it's not 0, the
		//       transition effect will cause focus to not transition properly to the new widgets when using
		//       gamepad always.
		LayerWidget->SetTransitionDuration(0.0);

		Layers.Add(LayerTag, LayerWidget);
	}
}

void UBinggyPrimaryGameLayout::OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase* Widget,
	bool bIsTransitioning)
{
	if (bIsTransitioning)
	{
		const FName SuspendToken = UCommonUIExtensions::SuspendInputForPlayer(GetOwningLocalPlayer(), TEXT("GlobalStackTransion"));
		SuspendInputTokens.Add(SuspendToken);
	}
	else
	{
		if (ensure(SuspendInputTokens.Num() > 0))
		{
			const FName SuspendToken = SuspendInputTokens.Pop();
			UCommonUIExtensions::ResumeInputForPlayer(GetOwningLocalPlayer(), SuspendToken);
		}
	}
}
