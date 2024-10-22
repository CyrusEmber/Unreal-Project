// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyWorldCollectable.h"


void ABinggyWorldCollectable::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	TArray<FInteractionOption>& Options)
{
	Options.Add_GetRef(Option);
}

FInventoryPickup ABinggyWorldCollectable::GetPickupInventory() const
{
	return StaticInventory;
}
