// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "BinggyInventoryItemDefinition.h"
#include "InventoryFragment_TradableItem.generated.h"

/**
 * 
 */
UCLASS()
class BINGGY_API UInventoryFragment_TradableItem : public UBinggyInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	float Value;

	// TODO: red pad color when you dont have enough money?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FLinearColor PadColor;
};
