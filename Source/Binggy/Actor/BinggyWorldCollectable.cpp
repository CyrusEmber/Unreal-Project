// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyWorldCollectable.h"


void ABinggyWorldCollectable::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& OptionBuilder)
{
	OptionBuilder.AddInteractionOption(Option);
}

FInventoryPickup ABinggyWorldCollectable::GetPickupInventory() const
{
	return StaticInventory;
}
