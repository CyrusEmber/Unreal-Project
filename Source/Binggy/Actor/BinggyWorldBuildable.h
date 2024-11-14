// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractableTarget.h"
#include "Engine/StaticMeshActor.h"
#include "BinggyWorldBuildable.generated.h"

/**
 *
 *	Defines current state of buildable actor.
 */
UENUM(BlueprintType)
enum class EBuildableState : uint8 {
	Pending UMETA(DisplayName = "Pending"),
	Building UMETA(DisplayName = "Under Construction"),
	Clear UMETA(DisplayName = "Not Overlapping, ready to be placed"),
	Blocked UMETA(DisplayName = "Overlapping With Others"),
	Placed UMETA(DisplayName = "Completed")   // the buildable completes placing in the world
};

/**
 * 
 */
UCLASS(Blueprintable)
class BINGGY_API ABinggyWorldBuildable : public AStaticMeshActor, public IInteractableTarget
{
	GENERATED_BODY()
public:
	ABinggyWorldBuildable();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;

	void SetBuildMesh(UStaticMesh* BuildMesh);

	UFUNCTION(BlueprintCallable, Category="Build")
	void UpdateMeshLocation(FVector TargetLocation);
	
	UFUNCTION(BlueprintCallable, Category="Build")
	void UpdateMeshRotation(FRotator TargetRotation);

	// Server execute function
	UFUNCTION(BlueprintCallable, Category="Build")
	void OnConstructionCompleted();

	// Server execute function
	UFUNCTION(BlueprintCallable, Category="Build")
	void OnConstructionBegin();

protected:
	// Turn red when there are overlap
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	

	// Client updates
	UFUNCTION()
	virtual void OnRep_BuildableState(EBuildableState OldBuildableState);

	// Respond to different states change.
	void SetBuildableState(EBuildableState NewBuildableState);

private:
	UPROPERTY(EditAnywhere, Category = "Default")
	FInteractionOption Option;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binggy|Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* RedOverlayMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binggy|Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* GreenOverlayMaterial;

	int32 OverlappingActorCount = 0;

	// Replicated state used to state change
	UPROPERTY(ReplicatedUsing = OnRep_BuildableState, VisibleAnywhere, Category = "Build")
	EBuildableState BuildableState;

	void SetMeshOverlayMaterial(UMaterialInterface* InOverlayMaterial);

	/*// The definition we use to create the item mesh
	UPROPERTY(EditAnywhere, Category = "Default")
	FBuildItemDefinition BuildItemDef;*/
};
