// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BinggyInventoryItemInstance.generated.h"

class UBinggyInventoryItemDefinition;
/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyInventoryItemInstance : public UObject
{
	GENERATED_BODY()

private:
	// TODO FGameplayTagStackContainer
	/*UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;*/

	UPROPERTY(Replicated)
	float Stack;

	// The item definition
	UPROPERTY(Replicated)
	TSubclassOf<UBinggyInventoryItemDefinition> ItemDef;
	
};
