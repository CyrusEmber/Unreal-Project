// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Styling/SlateBrush.h"
#include "BinggyInventoryItemDefinition.h"
#include "InventoryFragment_InventoryItem.generated.h"

class UObject;
/**
 * 
 */
UCLASS()
class BINGGY_API UInventoryFragment_InventoryItem : public UBinggyInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush Brush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	int32 MaximumStack;

	// TODO DebugOnly
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug|Pickup|Mesh")
	TObjectPtr<UStaticMesh> DebugDisplayMesh;
	
};
