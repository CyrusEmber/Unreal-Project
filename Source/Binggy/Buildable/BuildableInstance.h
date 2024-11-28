// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuildableInstance.generated.h"

class UBuildableDefinition;
enum class EBuildableSurfaceType : uint8;
class ABinggyWorldBuildable;
/**
 * The buildable spawned and owned by a pawn. Should handle initialization. It could be in the inventory.
 * It should only be in the server. Also, it handles a group of worldbuildables as attached actors.
 */
UCLASS(BlueprintType, Blueprintable)
class BINGGY_API UBuildableInstance : public UObject
{
	GENERATED_BODY()

public:
	UBuildableInstance();

	TSubclassOf<UBuildableDefinition> GetItemDef() const
	{
		return BuildableDef;
	}
	
	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	//~End of UObject interface
	
	// Blueprint-accessible getter for the owning actor
	UFUNCTION(BlueprintPure, Category = "Buildable")
	UObject* GetOwner() const { return Owner; }

	void SetOwner(UObject* InOwner) { Owner = InOwner; }

	// Buildable actor
	void SpawnBuildableActor(TSubclassOf<ABinggyWorldBuildable> BuildableClass,
		UStaticMesh* InBuildStaticMesh, TSet<EBuildableSurfaceType> AttachableSurfaces, FVector SpawnLocation);

	void DestroyBuildableActor();

	void SetBuildable(ABinggyWorldBuildable* InBuildable);

	void SetBuildableDef(const TSubclassOf<UBuildableDefinition>& InBuildableDef) { BuildableDef = InBuildableDef; }

	
private:
	UFUNCTION()
	void OnRep_Owner();

	// TODO: need ownership?
	UPROPERTY(ReplicatedUsing = OnRep_Owner)
	TObjectPtr<UObject> Owner;

	/*UPROPERTY(Replicated) TODO*/
	TObjectPtr<ABinggyWorldBuildable> Buildable;

	// The item definition
	UPROPERTY(Replicated)
	TSubclassOf<UBuildableDefinition> BuildableDef;

	
	
};
