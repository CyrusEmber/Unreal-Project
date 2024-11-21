// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BinggyEquipmentInstance.generated.h"

struct FBinggyEquipmentActorToSpawn;
/**
 * A piece of equipment spawned and applied to a pawn
 */
UCLASS()
class BINGGY_API UBinggyEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category=Equipment)
	APawn* GetPawn() const;
	
	virtual void SpawnEquipmentActors(const TArray<FBinggyEquipmentActorToSpawn>& ActorsToSpawn);
	virtual void DestroyEquipmentActors();

	virtual void OnEquipped();
	virtual void OnUnequipped();

private:
	UFUNCTION()
	void OnRep_Instigator();
	
private:
	UPROPERTY(ReplicatedUsing=OnRep_Instigator)
	TObjectPtr<UObject> Instigator;

	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;
	
};
