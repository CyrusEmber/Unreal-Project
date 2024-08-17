// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BinggyUserWidget.generated.h"

/**
 * One way relationshipt with widget controller
 */
class UBinggyWidgetController;

UCLASS()
class BINGGY_API UBinggyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	// I dont understand the usage
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

	
};
