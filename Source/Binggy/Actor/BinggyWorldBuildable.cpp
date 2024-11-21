// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyWorldBuildable.h"

#include "Net/UnrealNetwork.h"

ABinggyWorldBuildable::ABinggyWorldBuildable()
{
	// TODO: Change this when the build is done.
	PrimaryActorTick.bCanEverTick = true;
	// Spawn on the server and replicate to the client
	bReplicates = true;
	
	SetMobility(EComponentMobility::Movable);
	// Ready for overlap
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	// SetMobility(EComponentMobility::Static);
	// GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetStaticMeshComponent()->SetIsReplicated(true);
	/*bReplicates = true;
	// SetReplicates(true);
	SetReplicatingMovement(true);*/

	InitializeSnappingPoints();
}

void ABinggyWorldBuildable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABinggyWorldBuildable, BuildableState);
}

void ABinggyWorldBuildable::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
                                                     FInteractionOptionBuilder& OptionBuilder)
{
	OptionBuilder.AddInteractionOption(Option);
}

void ABinggyWorldBuildable::InitializeMeshAndOffset(UStaticMesh* InBuildStaticMesh)
{
	GetStaticMeshComponent()->SetStaticMesh(InBuildStaticMesh);
}

void ABinggyWorldBuildable::UpdatePreviewMeshPosition(const FVector& TargetLocation, const FVector& HitNormal, const FRotator& RotationAroundNormal)
{
	
	FVector ForwardVector = -HitNormal * PlacementOffset.Z;
	SetActorLocation(TargetLocation + ForwardVector);

	FRotator Rotation =  GetBaseRotation(HitNormal);
	FQuat NewQuat = FQuat(HitNormal, FMath::DegreesToRadians(RotationAroundNormal.Yaw));
	FRotator NewRotation = (NewQuat * Rotation.Quaternion()).Rotator();
	SetActorRotation(NewRotation);
}

FVector ABinggyWorldBuildable::FindNearestSnappingPoint(FVector TargetPosition)
{
	float MinDistance = FLT_MAX;
	FVector MinPoint;
	for (auto SnappingPoint : SnappingPoints)
	{
		FVector Location = SnappingPoint->GetComponentLocation();
		float Distance = FVector::Dist(TargetPosition, Location);
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			MinPoint = Location;
		}
	}
	return MinPoint;
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

FVector ABinggyWorldBuildable::CalculateOffsetSpawnPoint(AActor* CombinedActor)
{
	FVector ActorOrigin, ActorBoxExtent;
	CombinedActor->GetActorBounds(true, ActorOrigin, ActorBoxExtent);

	float MinZ = ActorOrigin.Z - ActorBoxExtent.Z;

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

void ABinggyWorldBuildable::InitializeSnappingPoints()
{
	// Origin is on the middle bottom point
	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(true, Origin, BoxExtent);

	float W = BoxExtent.X;
	float D = BoxExtent.Y;
	float H = BoxExtent.Z;

	FVector FrontCenter = Origin + FVector(0, D/2, H/2);

	FVector BackCenter = Origin - FVector(0, -D/2, H/2);

	FVector LeftCenter = Origin + FVector(-W/2, 0, H/2);

	FVector RightCenter = Origin + FVector(W/2, 0, H/2);

	FVector TopCenter = Origin + FVector(0, 0, H);

	FVector BottomCenter = Origin;

	USceneComponent* SnappingpointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MidpointComponent"));
	SnappingpointComponent->SetupAttachment(RootComponent);
	SnappingpointComponent->SetRelativeLocation(FrontCenter);

	USceneComponent* SnappingpointComponent1 = CreateDefaultSubobject<USceneComponent>(TEXT("MidpointComponent1"));
	SnappingpointComponent1->SetupAttachment(RootComponent);
	SnappingpointComponent1->SetRelativeLocation(BackCenter);

	USceneComponent* SnappingpointComponent2 = CreateDefaultSubobject<USceneComponent>(TEXT("MidpointComponent2"));
	SnappingpointComponent2->SetupAttachment(RootComponent);
	SnappingpointComponent2->SetRelativeLocation(RightCenter);

	USceneComponent* SnappingpointComponent3 = CreateDefaultSubobject<USceneComponent>(TEXT("MidpointComponent3"));
	SnappingpointComponent3->SetupAttachment(RootComponent);
	SnappingpointComponent3->SetRelativeLocation(LeftCenter);

	
}

void ABinggyWorldBuildable::AddSnappingPoint(USceneComponent* SnappingPoint)
{
	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(true, Origin, BoxExtent);
	
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
	// Generate overlap events only for server
	if (HasAuthority())
	{
		GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
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
