// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyWorldBuildable.h"

#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Net/UnrealNetwork.h"

ABinggyWorldBuildable::ABinggyWorldBuildable()
{
	// TODO: Change this when the build is done. This is not necessary gone for dynamic buildable.
	PrimaryActorTick.bCanEverTick = true;
	// Spawn on the server and replicate to the client
	bReplicates = true;
	
	SetMobility(EComponentMobility::Movable);
	// Ready for overlap
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);

	GetStaticMeshComponent()->SetIsReplicated(true);

	/*SetReplicates(true);
	SetReplicatingMovement(true);*/
	
}

void ABinggyWorldBuildable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABinggyWorldBuildable, BuildableState);
	DOREPLIFETIME(ABinggyWorldBuildable, TargetLocation);
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
	SetBuildableState(EBuildableState::Placed);
}

void ABinggyWorldBuildable::OnConstructionBegin()
{
	SetBuildableState(EBuildableState::Building);
	PlacementOffset = CalculateOffsetSpawnPoint(this);
}

void ABinggyWorldBuildable::OnSnappingBegin()
{
	SetBuildableState(EBuildableState::Snapping);
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
		// Async Load
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
			}));
		
		GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
	}
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
	if (BuildableState == OldBuildableState)
	{
		return;
	}
	
	SetBuildableState(BuildableState);
}

void ABinggyWorldBuildable::SetBuildableState(EBuildableState NewBuildableState)
{
	BuildableState = NewBuildableState;
	switch (NewBuildableState)
	{
		case (EBuildableState::Building):
			SetMeshOverlayMaterial(GreenOverlayMaterial);
			GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
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

		// Should not respond overlap events
		case (EBuildableState::Placed):
			GetStaticMeshComponent()->SetOverlayMaterial(nullptr);
			GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			// TODO set correct channel
			GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Block);
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
