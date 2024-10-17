// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "BinggyInventoryItemDefinition.generated.h"

class UBinggyInventoryItemInstance;
// Represents a fragment of an item definition
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class BINGGY_API UBinggyInventoryItemFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(UBinggyInventoryItemInstance* Instance) const {}
};

/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract)
class BINGGY_API UBinggyInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced)
	TArray<TObjectPtr<UBinggyInventoryItemFragment>> Fragments;

	// The background
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush Background;*/

	// TODO equipment?Description
	
};
