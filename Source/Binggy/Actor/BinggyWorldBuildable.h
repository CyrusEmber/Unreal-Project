// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Binggy/Interaction/InteractableTarget.h"
#include "Engine/StaticMeshActor.h"
#include "BinggyWorldBuildable.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BINGGY_API ABinggyWorldBuildable : public AStaticMeshActor, public IInteractableTarget
{
	GENERATED_BODY()
public:
	ABinggyWorldBuildable();

	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;

	void SetBuildMode();

	void SetBuildMesh(UStaticMesh* BuildMesh);

	UFUNCTION(BlueprintCallable, Category="Build")
	void UpdateMeshLocation(FVector TargetLocation);
	
	UFUNCTION(BlueprintCallable, Category="Build")
	void UpdateMeshRotation(FRotator TargetRotation);

protected:
	// Turn red when there are overlap
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Default")
	FInteractionOption Option;

	/*// The definition we use to create the item mesh
	UPROPERTY(EditAnywhere, Category = "Default")
	FBuildItemDefinition BuildItemDef;*/
};
