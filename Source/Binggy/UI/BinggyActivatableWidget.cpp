// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyActivatableWidget.h"

TOptional<FUIInputConfig> UBinggyActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case EBinggyWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case EBinggyWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case EBinggyWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
	case EBinggyWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
	
}
