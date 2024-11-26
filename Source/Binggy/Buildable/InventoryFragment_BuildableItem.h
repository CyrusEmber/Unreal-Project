#pragma once

#include "Inventory/BinggyInventoryItemDefinition.h"
#include "InventoryFragment_BuildableItem.generated.h"


class UBuildableDefinition;
UCLASS()
class BINGGY_API UInventoryFragment_BuildableItem : public UBinggyInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=Binggy)
	TSubclassOf<UBuildableDefinition> BuildableDefinition;

	
};
