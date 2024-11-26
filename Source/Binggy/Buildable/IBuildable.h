#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "IBuildable.generated.h"

class UBinggyInventoryItemDefinition;

USTRUCT(BlueprintType)
struct FBuildableDef
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBinggyInventoryItemDefinition> ItemDef;
};

class ABinggyWorldBuildable;
enum class EBuildableState : uint8;
// TODO: Combine with item definition
UENUM(BlueprintType)
enum class EResourceType : uint8
{
	Wood,
	Water,
};

USTRUCT(BlueprintType)
struct FBuildablePlaceHolder : public FTableRowBase
{
	GENERATED_BODY()

	// Unique ID or name for the buildable item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable")
	FText DisplayName;

	// Check buildable options and surfaces
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable")
	TSoftClassPtr<ABinggyWorldBuildable> BinggyBuildable;
 
	// Construction cost in resources
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable")
	TMap<EResourceType, int32> ConstructionCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable")
	TSoftObjectPtr<UTexture2D> Icon;

	// Time required to construct the buildable in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable")
	float ConstructionTime;
};

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UIBuildable : public UInterface
{
	GENERATED_BODY()
};

/**  */
class IIBuildable
{
	GENERATED_BODY()

public:
	// Get the private inventory pickup
	UFUNCTION(BlueprintCallable)
	virtual FBuildableDef GetBuildableDef() const = 0;
	
};
