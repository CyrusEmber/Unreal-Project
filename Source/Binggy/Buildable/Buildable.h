#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Buildable.generated.h"

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
struct FBuildable : public FTableRowBase
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
