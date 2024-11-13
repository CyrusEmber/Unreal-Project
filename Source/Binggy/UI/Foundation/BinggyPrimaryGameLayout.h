// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "BinggyPrimaryGameLayout.generated.h"

class UCommonActivatableWidget;
class UCommonActivatableWidgetContainerBase;
/**
 *  Stores the layout of different Stacks. And you can register layer with widget.
 *  This widget class represents how to layout, push and display all layers
 *  of the UI for a single player.
 *  The class is abstract and should implement a blueprint version.
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class BINGGY_API UBinggyPrimaryGameLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	static UBinggyPrimaryGameLayout* GetPrimaryGameLayoutForPrimaryPlayer(const UObject* WorldContextObject);
	static UBinggyPrimaryGameLayout* GetPrimaryGameLayout(APlayerController* PlayerController);
	static UBinggyPrimaryGameLayout* GetPrimaryGameLayout(ULocalPlayer* LocalPlayer);
	
public:
	UBinggyPrimaryGameLayout(const FObjectInitializer& ObjectInitializer);

public:
	// Push widget without initialize function
	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass)
	{
		return PushWidgetToLayerStack<ActivatableWidgetT>(LayerName, ActivatableWidgetClass, [](ActivatableWidgetT&) {});
	}

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass, TFunctionRef<void(ActivatableWidgetT&)> InitInstanceFunc)
	{
		static_assert(TIsDerivedFrom<ActivatableWidgetT, UCommonActivatableWidget>::IsDerived, "Only CommonActivatableWidgets can be used here");

		if (UCommonActivatableWidgetContainerBase* Layer = GetLayerWidget(LayerName))
		{
			return Layer->AddWidget<ActivatableWidgetT>(ActivatableWidgetClass, InitInstanceFunc);
		}

		return nullptr;
	}

	// Find the widget if it exists on any of the layers and remove it from the layer.
	void FindAndRemoveWidgetFromLayer(UCommonActivatableWidget* ActivatableWidget);

	// Get the layer widget for the given layer tag.
	UCommonActivatableWidgetContainerBase* GetLayerWidget(FGameplayTag LayerName);

protected:
	/** Register a layer that widgets can be pushed onto. */
	UFUNCTION(BlueprintCallable, Category="Layer")
	void RegisterLayer(UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget);

	void OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase* Widget, bool bIsTransitioning);

private:
	// Lets us keep track of all suspended input tokens so that multiple async UIs can be loading and we correctly suspend
	// for the duration of all of them.
	TArray<FName> SuspendInputTokens;

	// The registered layers for the primary layout.
	UPROPERTY(Transient, meta = (Categories = "UI.Layer"))
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
};
