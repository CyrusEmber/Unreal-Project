// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyUIPolicy.h"

#include "BinggyPrimaryGameLayout.h"
#include "UI/Subsystem/BinggyUIManagerSubsystem.h"

UBinggyUIPolicy* UBinggyUIPolicy::GetGameUIPolicy(const UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (UBinggyUIManagerSubsystem* UIManager = UGameInstance::GetSubsystem<UBinggyUIManagerSubsystem>(GameInstance))
			{
				return UIManager->GetCurrentUIPolicy();
			}
		}
	}

	return nullptr;
}

UWorld* UBinggyUIPolicy::GetWorld() const
{
	return GetOwningUIManager()->GetGameInstance()->GetWorld();
}

UBinggyUIManagerSubsystem* UBinggyUIPolicy::GetOwningUIManager() const
{
	return CastChecked<UBinggyUIManagerSubsystem>(GetOuter());
}

UBinggyPrimaryGameLayout* UBinggyUIPolicy::GetRootLayout(const ULocalPlayer* LocalPlayer) const
{
	const FBinggyRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer);
	return LayoutInfo ? LayoutInfo->RootLayout : nullptr;
}

void UBinggyUIPolicy::RequestPrimaryControl(UBinggyPrimaryGameLayout* Layout)
{
}

void UBinggyUIPolicy::AddLayoutToViewport(ULocalPlayer* LocalPlayer, UBinggyPrimaryGameLayout* Layout)
{
	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
	Layout->AddToPlayerScreen(1000);
}

void UBinggyUIPolicy::RemoveLayoutFromViewport(ULocalPlayer* LocalPlayer, UBinggyPrimaryGameLayout* Layout)
{
	TWeakPtr<SWidget> LayoutSlateWidget = Layout->GetCachedWidget();
	if (LayoutSlateWidget.IsValid())
	{
		Layout->RemoveFromParent();
		OnRootLayoutRemovedFromViewport(LocalPlayer, Layout);
	}
}

void UBinggyUIPolicy::OnRootLayoutAddedToViewport(ULocalPlayer* LocalPlayer, UBinggyPrimaryGameLayout* Layout)
{
}

void UBinggyUIPolicy::OnRootLayoutRemovedFromViewport(ULocalPlayer* LocalPlayer, UBinggyPrimaryGameLayout* Layout)
{
}

void UBinggyUIPolicy::OnRootLayoutReleased(ULocalPlayer* LocalPlayer, UBinggyPrimaryGameLayout* Layout)
{
}

void UBinggyUIPolicy::CreateLayoutWidget(ULocalPlayer* LocalPlayer)
{
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	{
		TSubclassOf<UBinggyPrimaryGameLayout> LayoutWidgetClass = GetLayoutWidgetClass(LocalPlayer);
		// Check if the class is a BP version
		if (ensure(LayoutWidgetClass && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			UBinggyPrimaryGameLayout* NewLayoutObject = CreateWidget<UBinggyPrimaryGameLayout>(PlayerController, LayoutWidgetClass);
			RootViewportLayouts.Emplace(LocalPlayer, NewLayoutObject, true);
			
			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
}

TSubclassOf<UBinggyPrimaryGameLayout> UBinggyUIPolicy::GetLayoutWidgetClass(ULocalPlayer* LocalPlayer)
{
	return LayoutClass.LoadSynchronous();
}

void UBinggyUIPolicy::NotifyPlayerAdded(ULocalPlayer* LocalPlayer)
{
	// Add binding to play
	LocalPlayer->OnPlayerControllerChanged().AddWeakLambda(this, [this, LocalPlayer](APlayerController* PlayerController)
	{
		NotifyPlayerRemoved(LocalPlayer);

		if (FBinggyRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
		{
			AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
			LayoutInfo->bAddedToViewport = true;
		}
		else
		{
			CreateLayoutWidget(LocalPlayer);
		}
	});
	// TODO On player controller set??
	// NotifyPlayerRemoved(LocalPlayer);

	if (FBinggyRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bAddedToViewport = true;
	}
	else
	{
		CreateLayoutWidget(LocalPlayer);
	}
	
	/*
	if (FBinggyRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bAddedToViewport = true;
	}
	else
	{
		CreateLayoutWidget(LocalPlayer);
	}*/

}

void UBinggyUIPolicy::NotifyPlayerRemoved(ULocalPlayer* LocalPlayer)
{
	if (FBinggyRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		RemoveLayoutFromViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bAddedToViewport = false;
	}
}

void UBinggyUIPolicy::NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);
	const int32 LayoutInfoIdx = RootViewportLayouts.IndexOfByKey(LocalPlayer);
	if (LayoutInfoIdx != INDEX_NONE)
	{
		UBinggyPrimaryGameLayout* Layout = RootViewportLayouts[LayoutInfoIdx].RootLayout;
		RootViewportLayouts.RemoveAt(LayoutInfoIdx);

		RemoveLayoutFromViewport(LocalPlayer, Layout);

		OnRootLayoutReleased(LocalPlayer, Layout);
	}
}
