// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyUserWidget.h"

void UBinggyUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}