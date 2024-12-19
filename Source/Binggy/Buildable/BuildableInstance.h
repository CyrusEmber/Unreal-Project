// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuildableInstance.generated.h"

class ABinggyWorldBuildable;
struct FBuildablePair
{
	ABinggyWorldBuildable* BuildableA;
	ABinggyWorldBuildable* BuildableB;

	FBuildablePair(ABinggyWorldBuildable* InA = nullptr, ABinggyWorldBuildable* InB = nullptr)
		: BuildableA(InA), BuildableB(InB) {}

	// Override equality operator
	bool operator==(const FBuildablePair& Other) const
	{
		return (BuildableA == Other.BuildableA && BuildableB == Other.BuildableB) ||
			   (BuildableA == Other.BuildableB && BuildableB == Other.BuildableA); // Handle unordered pair
	}
};

// Hash function for TMap
FORCEINLINE uint32 GetTypeHash(const FBuildablePair& Pair)
{
	// XOR the hash of both pointers to create a unique hash for the pair
	return HashCombine(GetTypeHash(Pair.BuildableA), GetTypeHash(Pair.BuildableB));
}

class APhysicsConstraintActor;
class UBuildableDefinition;
enum class EBuildableSurfaceType : uint8;
/**
 * The buildable spawned and owned by a pawn. Should handle initialization. It could be in the inventory? TODO?
 * It should only be in the server. Also, it handles a group of worldbuildables as attached actors.
 */
UCLASS(BlueprintType, Blueprintable)
class BINGGY_API UBuildableInstance : public UObject
{
	GENERATED_BODY()

public:
	UBuildableInstance();

	/** Add a buildable to this instance */
	UFUNCTION(BlueprintCallable, Category = "Buildable")
	void AddBuildable(ABinggyWorldBuildable* Buildable);

	/** Remove a buildable from this instance */
	UFUNCTION(BlueprintCallable, Category = "Buildable")
	void RemoveBuildable(ABinggyWorldBuildable* Buildable);
	
	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	//~End of UObject interface
	
	// Blueprint-accessible getter for the owning actor
	UFUNCTION(BlueprintPure, Category = "Buildable")
	UObject* GetOwner() const { return Owner; }

	void SetOwner(UObject* InOwner) { Owner = InOwner; }

	void DestroyBuildableActor();

	UFUNCTION(BlueprintCallable, Category = "Buildable")
	void AddConstraintBetweenBuildables(ABinggyWorldBuildable* Buildable1, ABinggyWorldBuildable* Buildable2);
	
	
private:
	UFUNCTION()
	void OnRep_Owner();

	// TODO: need ownership?
	UPROPERTY(ReplicatedUsing = OnRep_Owner)
	TObjectPtr<UObject> Owner;

	TArray<ABinggyWorldBuildable*> Buildables;

	TMap<FBuildablePair, APhysicsConstraintActor*> Constraints;

	
	
};
