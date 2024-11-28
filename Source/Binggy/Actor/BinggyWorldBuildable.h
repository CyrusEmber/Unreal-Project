// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractableTarget.h"
#include "Buildable/BuildableDefinition.h"
#include "Engine/StaticMeshActor.h"
#include "BinggyWorldBuildable.generated.h"

class USphereComponent;
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
	Snapping UMETA(DisplayName = "Snapping"),
	Placed UMETA(DisplayName = "Completed")   // the buildable completes placing in the world
};


/**
 * Should implement Buildable interface...
 * 
 */
UCLASS(Blueprintable)
class BINGGY_API ABinggyWorldBuildable : public AStaticMeshActor, public IInteractableTarget, public IIBuildable
{
	GENERATED_BODY()
public:
	ABinggyWorldBuildable();

	// Server execute function
	UFUNCTION(BlueprintCallable, Category="Buildable")
	void OnConstructionCompleted();

	UFUNCTION(BlueprintCallable, Category="Buildable")
	void OnConstructionBegin();

	UFUNCTION(BlueprintCallable, Category="Buildable")
	void OnSnappingBegin();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Begin of IInteractble interface
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
	// End of IInteractble interface.
	
	void InitializeMesh(UStaticMesh* InBuildStaticMesh);
	
	// Initialize the mesh at runtime.
	void SetAttachableSurfaces(const TSet<EBuildableSurfaceType>& InAttachableSurfaces) { AttachableSurfaces = InAttachableSurfaces; };
	
	/**
	 * Update target location and rotation
	 * @param NewLocation The new target location
	 * @param HitNormal The new hit result normal indicating which surface we want to place the mesh.
	 * @param RotationAroundNormal The only rotation allowed when creating the mesh
	 */
	void UpdatePreviewMeshPosition(const FVector& NewLocation, const FVector& HitNormal, const FRotator& RotationAroundNormal);

	// Start IBuildable Interface
	virtual TSubclassOf<UBuildableDefinition> GetBuildableDef() const override;
	// End IBuildable Interface
	
protected:
	// Turn red when there are overlap
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Buildable")
	FInteractionOption Option;

	UPROPERTY(EditAnywhere, Category = "Buildable")
	TSubclassOf<UBuildableDefinition> BuildableDef;

	// Client updates
	UFUNCTION()
	virtual void OnRep_BuildableState(EBuildableState OldBuildableState);

	// Respond to different states change.
	void SetBuildableState(EBuildableState NewBuildableState);

	
private:
	// Properties
	// Define a set of surfaces this buildable can attach to
	UPROPERTY(EditDefaultsOnly, Category = "Buildable")
	TSet<EBuildableSurfaceType> AttachableSurfaces;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable|Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* RedOverlayMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable|Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* GreenOverlayMaterial;

	int32 OverlappingActorCount = 0;

	// Replicated state used to state change
	UPROPERTY(ReplicatedUsing = OnRep_BuildableState, VisibleAnywhere, Category = "Build")
	EBuildableState BuildableState;

	void SetMeshOverlayMaterial(UMaterialInterface* InOverlayMaterial);

	// The offset from the initial spawn point (actor center) and the actual spawn point.
	FVector CalculateOffsetSpawnPoint(AActor* CombinedActor);

	// Update the rotation based on HitResult's norm and EBuildableSurfaceType, clamping is made. With a tolerance
	FRotator GetBaseRotation(const FVector& HitNormal, const float VerticalTolerance = 0.01f);

	// Helper function
	bool CanAttachToSurface(EBuildableSurfaceType SurfaceType) const
	{
		return AttachableSurfaces.Contains(SurfaceType);
	}

	FVector PlacementOffset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Buildable|Debug", meta = (AllowPrivateAccess = "true"))
	bool Debug = true;

	UPROPERTY(Replicated)
	FVector TargetLocation;

	UPROPERTY(Replicated)
	UBuildableInstance* BuildableInstance;

	

	// TODO
	/*// The definition we use to create the item mesh
	UPROPERTY(EditAnywhere, Category = "Default")
	FBuildItemDefinition BuildItemDef;*/
};
