// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/BinggyWorldBuildable.h"
#include "Components/ActorComponent.h"
#include "BinggyBuildableManagerComponent.generated.h"


class UBuildableDefinition;
class UBuildableInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BINGGY_API UBinggyBuildableManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBinggyBuildableManagerComponent();

	// Spawn buildable in the server TODO buildable instance multiple attached buildable
	// Spawned buildable at the owner location, then send the gameplay event BuildActive to the owner
	UFUNCTION(BlueprintCallable)
	void CreateAndActivateBuildable(TSubclassOf<ABinggyWorldBuildable> BuildableClass);

	UFUNCTION(Server, Reliable)
	void ServerCreateAndActivateBuildable(TSubclassOf<ABinggyWorldBuildable> BuildableClass);

	// Server only function, assume child is a single block
	UFUNCTION(BlueprintCallable)
	void ConnectTwoBuildables(ABinggyWorldBuildable* Parent, ABinggyWorldBuildable* Child);

	void AttachTwoBuildables(ABinggyWorldBuildable* Parent, ABinggyWorldBuildable* Child);
	
	void AddConstraintBetweenBuildables(ABinggyWorldBuildable* Parent, ABinggyWorldBuildable* Child);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(ReplicatedUsing=OnRep_CurrentBuildable)
	ABinggyWorldBuildable* CurrentBuildable;

	UFUNCTION()
	void OnRep_CurrentBuildable();

		
};
