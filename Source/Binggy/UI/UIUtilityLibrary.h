// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetController/OverlayWidgetController.h"
#include "UIUtilityLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BINGGY_API UUIUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Called from a widget with a player controller
	UFUNCTION(BlueprintPure, Category = "UILibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
};
