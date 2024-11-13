// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BinggyActivatableWidget.h"
#include "BinggyActivatableMenu.generated.h"

/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyActivatableMenu : public UBinggyActivatableWidget
{
	GENERATED_BODY()

	// TODO: now it serve the same functionality as BinggyUserWidget
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	// It generates an event when set the widget controller
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
};
