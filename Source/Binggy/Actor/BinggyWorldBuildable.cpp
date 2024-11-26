// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyWorldBuildable.h"

#include "Components/SphereComponent.h"
#include "Rendering/PositionVertexBuffer.h"
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

	// Add snapping points
	for (int32 i = 0; i < 6; ++i)
	{
		FName ComponentName = FName(*FString::Printf(TEXT("SnappingPoint_%d"), i));
		USphereComponent* SnappingSphere = CreateDefaultSubobject<USphereComponent>(ComponentName);
		SnappingSphere->SetupAttachment(RootComponent);
		SnappingSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		SnappingSphere->SetGenerateOverlapEvents(true);
		SnappingSphere->InitSphereRadius(0.f); // Not tamping with the box extent TODO
		SnappingSpheres.Add(SnappingSphere);
		SnappingSphere->RegisterComponent();
	}
	
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
		FVector Location = SnappingPoint;
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

void ABinggyWorldBuildable::InitializeSnappingPoints()
{
	// Origin is on the actor center
	FVector Origin;
	FVector BoxExtent;
	
	GetActorBounds(true, Origin, BoxExtent);

	TArray<FVector> RelativeLocations = {
	    Origin + FVector(0,  BoxExtent.Y, 0),  // FrontCenter
	    Origin + FVector(0, -BoxExtent.Y, 0),  // BackCenter
	    Origin + FVector(-BoxExtent.X, 0, 0),  // LeftCenter
	    Origin + FVector(BoxExtent.X,  0, 0),  // RightCenter
	    Origin + FVector(0, 0, BoxExtent.Z),   // TopCenter
	    Origin + FVector(0, 0, -BoxExtent.Z)   // Bottom Center
	};
	
	for (int32 i = 0; i < RelativeLocations.Num(); ++i)
	{
		SnappingSpheres[i]->SetWorldLocation(RelativeLocations[i]);
		SnappingSpheres[i]->InitSphereRadius(SnappingRadius); // Set the desired radius
		SnappingPoints.Add(RelativeLocations[i]);
#if WITH_EDITOR
		if (Debug)
		{
			DrawDebugSphere(
			GetWorld(),
			SnappingSpheres[i]->GetComponentLocation(),
			30.0f,             // Radius
			12,                // Segments
			FColor::Green,     // Color
			true          // Persistent (false means it will disappear after a short time)
		);
#endif
		}
		
	}
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

	

	InitializeSnappingPoints();
	for (auto SnappingPoint : SnappingPoints)
	{
		FColor PointColor = FColor::Red;
		float PointSize = 10.0f;
		DrawDebugPoint(GetWorld(), SnappingPoint, PointSize, PointColor, true);
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
