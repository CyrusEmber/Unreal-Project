// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "BinggyButtonBase.generated.h"

/**
 * To set the button text, override ButtonText 
 */
UCLASS()
class BINGGY_API UBinggyButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(const FText& InText);
	
protected:
	// UUserWidget interface
	virtual void NativePreConstruct() override;
	// End of UUserWidget interface

	// UCommonButtonBase interface
	virtual void UpdateInputActionWidget() override;
	// When the input such as keyboard, Gamepad changed
	virtual void OnInputMethodChanged(ECommonInputType CurrentInputType) override;
	// End of UCommonButtonBase interface

	void RefreshButtonText();

	// Blueprint implemented event
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonText(const FText& InText);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonStyle();
	// End of Blueprint implemented event
	
private:
	UPROPERTY(EditAnywhere, Category="Button", meta=(InlineEditConditionToggle))
	uint8 bOverride_ButtonText : 1;
	
	UPROPERTY(EditAnywhere, Category="Button", meta=( editcondition="bOverride_ButtonText" ))
	FText ButtonText;
	
};
