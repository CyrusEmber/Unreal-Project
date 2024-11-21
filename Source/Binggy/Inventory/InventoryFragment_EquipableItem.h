#pragma once
#include "BinggyInventoryItemDefinition.h"
#include "InventoryFragment_EquipableItem.generated.h"

class UBinggyEquipmentDefinition;
class UObject;
/**
 * 
 */
UCLASS()
class BINGGY_API UInventoryFragment_EquipableItem : public UBinggyInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Equippment")
	TSubclassOf<UBinggyEquipmentDefinition> EquipmentDefinition;
	
};
