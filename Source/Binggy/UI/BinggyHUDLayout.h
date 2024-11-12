// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BinggyActivatableWidget.h"
#include "BinggyHUDLayout.generated.h"

/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyHUDLayout : public UBinggyActivatableWidget
{
	GENERATED_BODY()
public:
	void NativeOnInitialized() override;

protected:
	void HandleEscapeAction();

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;
	
};
