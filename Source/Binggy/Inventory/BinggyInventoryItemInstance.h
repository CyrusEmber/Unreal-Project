// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BinggyInventoryItemInstance.generated.h"

class UBinggyInventoryItemDefinition;
/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	UBinggyInventoryItemInstance();
	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	//~End of UObject interface

	TSubclassOf<UBinggyInventoryItemDefinition> GetItemDef() const
	{
		return ItemDef;
	}

private:
	void SetItemDef(TSubclassOf<UBinggyInventoryItemDefinition> InDef);

	friend struct FBinggyInventoryList;

	
	
	// TODO FGameplayTagStackContainer
	/*UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;*/

	// The item definition
	UPROPERTY(Replicated)
	TSubclassOf<UBinggyInventoryItemDefinition> ItemDef;
	
};
