// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BinggyEquipmentDefinition.generated.h"

class UBinggyEquipmentInstance;

USTRUCT()
struct FBinggyEquipmentActorToSpawn
{
	GENERATED_BODY()

	FBinggyEquipmentActorToSpawn()
	{}

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};

/**
 * Definition of a piece of equipment that can be applied to a pawn
 */
UCLASS()
class BINGGY_API UBinggyEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<UBinggyEquipmentInstance> InstanceType;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<FBinggyEquipmentActorToSpawn> ActorsToSpawn;
	
};
