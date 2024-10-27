// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BinggyInventoryItemDefinition.h"
#include "InventoryFragment_ConsumableItem.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class BINGGY_API UInventoryFragment_ConsumableItem : public UBinggyInventoryItemFragment
{
	GENERATED_BODY()

public:
	// TODO: use it to others, but use it in inventory should only apply the effect to self
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Fragment|Attribute")
	TSubclassOf<UGameplayEffect> ConsumableItemEffectClass;

	// TODO potentially
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Fragment|Attribute")
	TSubclassOf<UGameplayEffect> ConsumableItemCooldownEffectClass;
	
};
