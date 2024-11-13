// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Foundation/BinggyActivatableMenu.h"

void UBinggyActivatableMenu::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
