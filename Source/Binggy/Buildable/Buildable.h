#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Buildable.generated.h"

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
	FName Name;

	// Static mesh associated with this buildable item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable")
	UStaticMesh* BuildableMesh;

	// Material or overlay material for this buildable item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable")
	UMaterialInterface* OverlayMaterial;

	// Construction cost in resources (e.g., wood, stone)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable")
	int32 ConstructionCost;

	// Time required to construct the buildable in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable")
	float ConstructionTime;

	// Status of the buildable item (default should be Pending)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable")
	EBuildableState DefaultStatus;
};
