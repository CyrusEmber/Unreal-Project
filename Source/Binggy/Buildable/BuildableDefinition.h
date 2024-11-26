// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IBuildable.h"
#include "UObject/NoExportTypes.h"
#include "BuildableDefinition.generated.h"

class ABinggyWorldBuildable;
/**
 *	Defines the blocks it can attach to.
 */
UENUM(BlueprintType)
enum class EBuildableSurfaceType : uint8 {
	Floor,
	Wall,
	Ceiling,
	NonEvenSurface
};

class UBuildableInstance;
/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class BINGGY_API UBuildableDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Buildable")
	TSoftClassPtr<UBuildableInstance> InstanceType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Buildable")
	EBuildableSurfaceType AttachableSurfaces;

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
