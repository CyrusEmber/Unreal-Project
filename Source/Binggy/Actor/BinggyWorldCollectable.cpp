// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyWorldCollectable.h"

#include "Components/WidgetComponent.h"


ABinggyWorldCollectable::ABinggyWorldCollectable()
{
	// Set up UI WidgetComponent
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
	
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawAtDesiredSize(true);
	
}

void ABinggyWorldCollectable::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
                                                       FInteractionOptionBuilder& OptionBuilder)
{
	OptionBuilder.AddInteractionOption(Option);
}

FInventoryPickup ABinggyWorldCollectable::GetPickupInventory() const
{
	return StaticInventory;
}
