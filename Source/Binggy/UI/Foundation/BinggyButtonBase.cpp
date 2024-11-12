// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyButtonBase.h"
#include "CommonActionWidget.h"

void UBinggyButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateButtonStyle();
	RefreshButtonText();
}

void UBinggyButtonBase::UpdateInputActionWidget()
{
	Super::UpdateInputActionWidget();
	UpdateButtonStyle();
	RefreshButtonText();
}

void UBinggyButtonBase::OnInputMethodChanged(ECommonInputType CurrentInputType)
{
	Super::OnInputMethodChanged(CurrentInputType);
	UpdateButtonStyle();
}

void UBinggyButtonBase::SetButtonText(const FText& InText)
{
	// If the input is empty then the button text can be overridden.
	bOverride_ButtonText = InText.IsEmpty();
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, InText.ToString());
	ButtonText = InText;
	RefreshButtonText();
}

void UBinggyButtonBase::RefreshButtonText()
{
	if (bOverride_ButtonText || ButtonText.IsEmpty())
	{
		if (InputActionWidget)
		{
			// Get text for additional input actions?
			const FText ActionDisplayText = InputActionWidget->GetDisplayText();	
			if (!ActionDisplayText.IsEmpty())
			{
				UpdateButtonText(ActionDisplayText);
				return;
			}
		}
	}
	
	UpdateButtonText(ButtonText);	
}
