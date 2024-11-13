// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BinggyUIManagerSubsystem.generated.h"

class UCommonActivatableWidget;
class UBinggyActivatableWidget;
class UCommonLocalPlayer;
class UBinggyUIPolicy;
/**
 * 
 */
UCLASS(config = Game)
class BINGGY_API UBinggyUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	
	const UBinggyUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }
	UBinggyUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }

	// Add primary game layout to local player
	virtual void NotifyPlayerAdded(ULocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(ULocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer);

	// TODO use aync action, or add some init function
	// Push Widget to certain layer
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass);

	// Remove the first widget from the layers
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void RemoveWidgetFromLayer(UCommonActivatableWidget* ActivatableWidget);

protected:
	void SwitchToPolicy(UBinggyUIPolicy* InPolicy);

	// Assume the game is single player for each client, enough for now
	ULocalPlayer* GetFirstLocalPlayer();

private:
	UPROPERTY(Transient)
	TObjectPtr<UBinggyUIPolicy> CurrentPolicy = nullptr;

	UPROPERTY(config, EditAnywhere)
	TSoftClassPtr<UBinggyUIPolicy> DefaultUIPolicyClass;

	// TODO: there could be multiple widgets
	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftClassPtr<UBinggyActivatableWidget>> HUDClasses;

	// TODO: Menu Widget, should be a tab menu
	UPROPERTY()
	TSoftClassPtr<UBinggyActivatableWidget> GameMenuClass;

	UPROPERTY(config)
	TSubclassOf<UBinggyActivatableWidget> GameMenuClassPtr;


	// TODO Create a UCommonGameDialog for the dialog widgets.
	UPROPERTY()
	TSubclassOf<UBinggyActivatableWidget> ConfirmationDialogClassPtr;

	UPROPERTY()
	TSubclassOf<UBinggyActivatableWidget> ErrorDialogClassPtr;

	UPROPERTY(config)
	TSoftClassPtr<UBinggyActivatableWidget> ConfirmationDialogClass;

	UPROPERTY(config)
	TSoftClassPtr<UBinggyActivatableWidget> ErrorDialogClass;
	
};

UCLASS()
class BINGGY_API UUIBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// Get the subsystem in the game instance and add the widget to the specific layer
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	static void PushWidgetToLayerStack(APlayerController* PC, FGameplayTag LayerName, UClass* ActivatableWidgetClass);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	static void RemoveWidgetFromLayer(APlayerController* PC, UCommonActivatableWidget* ActivatableWidget);

private:
	static UBinggyUIManagerSubsystem* GetUIManagerSubsystem(APlayerController* PC);
};
