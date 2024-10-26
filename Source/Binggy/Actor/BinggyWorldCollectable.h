// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Binggy/Interaction/InteractableTarget.h"
#include "Binggy/Interaction/InteractionOption.h"
#include "Binggy/Inventory/Pickupable.h"
#include "GameFramework/Actor.h"
#include "BinggyWorldCollectable.generated.h"

UCLASS(Abstract, Blueprintable)
class BINGGY_API ABinggyWorldCollectable : public AActor, public IInteractableTarget, public IPickupable
{
	GENERATED_BODY()
	
public:	
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
	virtual FInventoryPickup GetPickupInventory() const override;

	// TODO: setup static mesh using the definition?
protected:
	UPROPERTY(EditAnywhere, Category = "Default")
	FInteractionOption Option;

	UPROPERTY(EditAnywhere, Category = "Default")
	FInventoryPickup StaticInventory;

};
