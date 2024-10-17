// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "BinggyInventoryItemDefinition.h"
#include "InventoryFragment_PickupItem.generated.h"

/**
 * 
 */
UCLASS()
class BINGGY_API UInventoryFragment_PickupItem : public UBinggyInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FLinearColor PadColor;
	
};
