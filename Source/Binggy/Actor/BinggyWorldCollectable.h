// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interaction/InteractableTarget.h"
#include "Interaction/InteractionOption.h"
#include "Inventory/IPickupable.h"
#include "GameFramework/Actor.h"
#include "BinggyWorldCollectable.generated.h"

class UWidgetComponent;

UCLASS(Abstract, Blueprintable)
class BINGGY_API ABinggyWorldCollectable : public AActor, public IInteractableTarget, public IIPickupable
{
	GENERATED_BODY()
	
public:
	ABinggyWorldCollectable();
	
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
	virtual FInventoryPickup GetPickupInventory() const override;

	// Widget Component to hold the 2D UI, Not used? TODO
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* WidgetComponent;

	// TODO: setup static mesh using the definition?
protected:
	UPROPERTY(EditAnywhere, Category = "Default")
	FInteractionOption Option;

	UPROPERTY(EditAnywhere, Category = "Default")
	FInventoryPickup StaticInventory;

};
