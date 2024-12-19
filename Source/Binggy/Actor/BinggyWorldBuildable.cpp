// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyWorldBuildable.h"

#include "Buildable/BuildableInstance.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Net/UnrealNetwork.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

ABinggyWorldBuildable::ABinggyWorldBuildable()
{
	// TODO: Change this when the build is done. This is not necessary gone for dynamic buildable.
	PrimaryActorTick.bCanEverTick = true;
	// Spawn on the server and replicate to the client
	bReplicates = true;

	// Default Settings
	SetMobility(EComponentMobility::Movable);
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	/*GetStaticMeshComponent()->SetIsReplicated(true);
	// TODO set correct channel
	GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Block);*/

	// Setup Physics Constraint
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(RootComponent);
	
	/*SetReplicates(true);
	SetReplicatingMovement(true);*/
	
}

void ABinggyWorldBuildable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABinggyWorldBuildable, BuildableState);
	// TODO set owner only
	DOREPLIFETIME(ABinggyWorldBuildable, TargetLocation);
	DOREPLIFETIME(ABinggyWorldBuildable, Neighbors);
}

void ABinggyWorldBuildable::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
                                                     FInteractionOptionBuilder& OptionBuilder)
{
	OptionBuilder.AddInteractionOption(Option);
}

void ABinggyWorldBuildable::InitializeMesh(UStaticMesh* InBuildStaticMesh)
{
	GetStaticMeshComponent()->SetStaticMesh(InBuildStaticMesh);
}

void ABinggyWorldBuildable::UpdatePreviewMeshPosition(const FVector& NewLocation, const FVector& HitNormal, const FRotator& RotationAroundNormal)
{
	
	FVector ForwardVector = -HitNormal * PlacementOffset.Z;
	TargetLocation = NewLocation + ForwardVector;

	FRotator Rotation =  GetBaseRotation(HitNormal);
	FQuat NewQuat = FQuat(HitNormal, FMath::DegreesToRadians(RotationAroundNormal.Yaw));
	FRotator NewRotation = (NewQuat * Rotation.Quaternion()).Rotator();
	SetActorRotation(NewRotation);
}

TSubclassOf<UBuildableDefinition> ABinggyWorldBuildable::GetBuildableDef() const
{
	return BuildableDef;
}

void ABinggyWorldBuildable::OnConstructionCompleted()
{
	if (HasAuthority())
	{
		SetBuildableState(EBuildableState::Placed);
	}
	
}

void ABinggyWorldBuildable::OnConstructionBegin()
{
	if (HasAuthority())
	{
		SetBuildableState(EBuildableState::Building);
		PlacementOffset = CalculateOffsetSpawnPoint(this);
	}
}

void ABinggyWorldBuildable::OnSnappingBegin()
{
	if (HasAuthority())
	{
		SetBuildableState(EBuildableState::Snapping);
	}
}

FVector ABinggyWorldBuildable::CalculateOffsetSpawnPoint(AActor* CombinedActor)
{
	FVector ActorOrigin, ActorBoxExtent;
	CombinedActor->GetActorBounds(true, ActorOrigin, ActorBoxExtent);
	
	FVector PivotLocation = GetActorLocation();

	// Box Extent is wrong?? TODO
	float MinZ = 0;

	/*for (UActorComponent* Component : CombinedActor->GetComponents())
	{
		if (UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Component))
		{
			FVector LocalOrigin, LocalBoxExtent;
			MeshComponent->GetLocalBounds(LocalOrigin, LocalBoxExtent);

			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("LocalBoxExtent: %s"), *LocalBoxExtent.ToString()));
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("LocalOrigin: %s"), *LocalOrigin.ToString()));
		}
	}*/

	// TODO: FIXME
	/*for (UActorComponent* Component : CombinedActor->GetComponents())
	{
		if (UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Component))
		{
			FVector LocalOrigin, LocalBoxExtent;
			MeshComponent->GetLocalBounds(LocalOrigin, LocalBoxExtent);

			// Calculate the local-space bottom point
			FVector LocalBottomPoint = LocalOrigin - FVector(0, 0, LocalBoxExtent.Z);

			// Transform the local bottom point to world space
			FVector WorldBottomPoint = MeshComponent->GetComponentTransform().TransformPosition(LocalBottomPoint);

			// Update MinZ if this component's bottom is lower
			if (WorldBottomPoint.Z < MinZ)
			{
				MinZ = WorldBottomPoint.Z;
			}
		}
	}*/

	return FVector(ActorOrigin.X, ActorOrigin.Y, MinZ);
}

FRotator ABinggyWorldBuildable::GetBaseRotation(const FVector& HitNormal, const float VerticalTolerance)
{
	FRotator PlacementRotation;
	if (FMath::IsNearlyEqual(HitNormal.Z, 1.0f, VerticalTolerance)) {
		// This is a floor
		if (CanAttachToSurface(EBuildableSurfaceType::Floor))
		{
			PlacementRotation = FRotator(0, 0, 0); 
		}
	} else if (FMath::IsNearlyEqual(HitNormal.Z, -1.0f, VerticalTolerance)) {
		// This is a ceiling
		if (CanAttachToSurface(EBuildableSurfaceType::Ceiling))
		{
			PlacementRotation = FRotator(180, 0, 0); 
		}
	} else if (FMath::IsNearlyZero(HitNormal.Z, VerticalTolerance)) {
		// This is a wall
		if (CanAttachToSurface(EBuildableSurfaceType::Wall))
		{
			PlacementRotation = HitNormal.Rotation();
			PlacementRotation.Pitch = 0; 
		}
	} else {
		// Possibly a sloped or uneven surface
		if (CanAttachToSurface(EBuildableSurfaceType::NonEvenSurface))
		{
			PlacementRotation = HitNormal.Rotation();
		}
	}
	return PlacementRotation;
}

void ABinggyWorldBuildable::SetupConstraint(ABinggyWorldBuildable* Neighbor)
{
	UPhysicsConstraintComponent* ConstraintComp = NewObject<UPhysicsConstraintComponent>(this);
	if (ConstraintComp)
	{
		// Attach to the root component or any desired component of the actor
		ConstraintComp->SetupAttachment(GetRootComponent());

		// Register the component so it becomes part of the actor's hierarchy
		ConstraintComp->RegisterComponent();

		// Optional: Name the component for debugging purposes
		// ConstraintComp->Rename(TEXT("DynamicPhysicsConstraint"));
	}
	
	if (!ConstraintComp || !Neighbor)
	{
		return;
	}

	// Set the constraint components
	ConstraintComp->SetConstrainedComponents(
		GetStaticMeshComponent(),   // This buildable's mesh
		NAME_None,                  // No specific bone
		Neighbor->GetStaticMeshComponent(), // Neighbor's mesh
		NAME_None                   // No specific bone
	);

	// Example configuration of the constraint
	ConstraintComp->SetLinearXLimit(LCM_Locked, 0.0f);
	ConstraintComp->SetLinearYLimit(LCM_Locked, 0.0f);
	ConstraintComp->SetLinearZLimit(LCM_Locked, 0.0f);

	ConstraintComp->SetAngularSwing1Limit(ACM_Locked, 0.0f);
	ConstraintComp->SetAngularSwing2Limit(ACM_Locked, 0.0f);
	ConstraintComp->SetAngularTwistLimit(ACM_Locked, 0.0f);
}

void ABinggyWorldBuildable::AddNeighbor(ABinggyWorldBuildable* Neighbor)
{
	if (Neighbor && Neighbor != this && !Neighbors.Contains(Neighbor))
	{
		Neighbors.Add(Neighbor);
		// Neighbor->AddNeighbor(this); // Ensure bidirectional relationship TODO
	}
	SetupConstraint(Neighbor);
}

void ABinggyWorldBuildable::RemoveNeighbor(ABinggyWorldBuildable* Neighbor)
{
	if (Neighbor)
	{
		Neighbors.Remove(Neighbor);
		Neighbor->Neighbors.Remove(this);
	}
}

void ABinggyWorldBuildable::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (BuildableState == EBuildableState::Placed)
	{
		return;
	}
	
#if WITH_EDITOR
	if (!HasAuthority()) // Server-only
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("NotifyActorBeginOverlap Execute in client! Bug!")); 
	}
#endif
	

	OverlappingActorCount++;
	SetBuildableState(EBuildableState::Blocked);
	
}

void ABinggyWorldBuildable::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (BuildableState == EBuildableState::Placed)
	{
		return;
	}

	OverlappingActorCount--;
	if (OverlappingActorCount == 0)
	{
		SetBuildableState(EBuildableState::Clear);
	}
	
}

void ABinggyWorldBuildable::BeginPlay()
{
	Super::BeginPlay();

	// TODO: initial states
	TargetLocation = GetActorLocation();
	
	// Generate overlap events only for server
	if (HasAuthority())
	{
		// Initial setup
		SetBuildableState(EBuildableState::Placed);
		/*// Async Load
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		TSoftObjectPtr<UStaticMesh> SoftMesh = GetDefault<UBuildableDefinition>(BuildableDef)->BuildableMesh;
		// Request asynchronous load
		Streamable.RequestAsyncLoad(SoftMesh.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda([this, SoftMesh]()
			{
				if (UStaticMesh* Mesh = SoftMesh.Get())
				{
					GetStaticMeshComponent()->SetStaticMesh(Mesh);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to load mesh"));
				}
			}));*/
		
		GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
	}
}

void ABinggyWorldBuildable::BeginDestroy()
{
	Super::BeginDestroy();
	
	for (ABinggyWorldBuildable* Neighbor : Neighbors)
	{
		if (Neighbor)
		{
			Neighbor->RemoveNeighbor(this);
		}
	}

	Neighbors.Empty();
}

void ABinggyWorldBuildable::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// Update the mesh location
	if ((BuildableState != EBuildableState::Snapping && BuildableState != EBuildableState::Placed)
		&& GetActorLocation() != TargetLocation)
	{
		FVector SmoothedLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaSeconds, 10.0f);
		SetActorLocation(SmoothedLocation);
	}

}

void ABinggyWorldBuildable::OnRep_BuildableState(EBuildableState OldBuildableState)
{	
	SetBuildableState(BuildableState);
}

void ABinggyWorldBuildable::SetBuildableState(EBuildableState NewBuildableState)
{
	BuildableState = NewBuildableState;
	switch (NewBuildableState)
	{
		case (EBuildableState::Building):
			SetMeshOverlayMaterial(GreenOverlayMaterial);
			GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
			GetStaticMeshComponent()->SetSimulatePhysics(false);
			if (HasAuthority())
			{
				GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
			}
			GetStaticMeshComponent()->SetCastShadow(false);
			break;

		case (EBuildableState::Clear):
			SetMeshOverlayMaterial(GreenOverlayMaterial);
			break;

		case (EBuildableState::Blocked):
			SetMeshOverlayMaterial(RedOverlayMaterial);
			break;

		case (EBuildableState::Snapping):
			// TODO: set the snapping
			break;

		// Should not respond overlap events
		case (EBuildableState::Placed):
			GetStaticMeshComponent()->SetOverlayMaterial(nullptr);			
			// TODO set correct channel
			GetStaticMeshComponent()->SetCollisionProfileName("BlockAll");
			GetStaticMeshComponent()->SetSimulatePhysics(true);
			if (HasAuthority())
			{
				GetStaticMeshComponent()->SetGenerateOverlapEvents(false);
			}
			GetStaticMeshComponent()->SetCastShadow(true);
			break;

		default:
			break;
	}
}


void ABinggyWorldBuildable::SetMeshOverlayMaterial(UMaterialInterface* InOverlayMaterial)
{
	GetStaticMeshComponent()->SetOverlayMaterial(InOverlayMaterial);
}
