// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/DamageTextWidgetComponent.h"

UDamageTextWidgetComponent::UDamageTextWidgetComponent()
{
	SetWidgetSpace(EWidgetSpace::Screen);
	bDrawAtDesiredSize = true;
}

void UDamageTextWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	check(WidgetClass);
}


