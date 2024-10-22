// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitForInteractableTarget.h"

#include "Abilities/GameplayAbilityTargetTypes.h"

void UWaitForInteractableTarget::Activate()
{
	SetWaitingOnAvatar();

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::PerformTrace, InteractionScanRate, true);
}


void UWaitForInteractableTarget::OnDestroy(bool AbilityEnded)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}
	Super::OnDestroy(AbilityEnded);
}

UWaitForInteractableTarget* UWaitForInteractableTarget::WaitForInteractableTarget(UGameplayAbility* OwningAbility,
	FCollisionProfileName TraceProfile, FInteractionQuery InInteractionQuery, FGameplayAbilityTargetingLocationInfo StartLocation, float InteractionScanRange,
	float InteractionScanRate, bool bShowDebug)
{
	UWaitForInteractableTarget* MyObj = NewAbilityTask<UWaitForInteractableTarget>(OwningAbility);
	MyObj->InteractionScanRange = InteractionScanRange;
	MyObj->InteractionScanRate = InteractionScanRate;
	MyObj->StartLocation = StartLocation;
	MyObj->TraceProfile = TraceProfile;
	MyObj->InteractionQuery = InInteractionQuery;
	MyObj->bShowDebug = bShowDebug;

	return MyObj;
}

void UWaitForInteractableTarget::AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params,
	const FVector& TraceStart, float MaxRange, FVector& OutTraceEnd, bool bIgnorePitch) const
{
	if (!Ability) // Server and launching client only
	{
		return;
	}

	//@TODO: Bots?
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	check(PC);

	FVector ViewStart;
	FRotator ViewRot;
	PC->GetPlayerViewPoint(ViewStart, ViewRot);

	const FVector ViewDir = ViewRot.Vector();
	FVector ViewEnd = ViewStart + (ViewDir * MaxRange);

	ClipCameraRayToAbilityRange(ViewStart, ViewDir, TraceStart, MaxRange, ViewEnd);

	FHitResult HitResult;
	LineTrace(HitResult, InSourceActor->GetWorld(), ViewStart, ViewEnd, TraceProfile.Name, Params);
	
	const bool bUseTraceResult = HitResult.bBlockingHit && (FVector::DistSquared(TraceStart, HitResult.Location) <= (MaxRange * MaxRange));
	
	const FVector AdjustedEnd = (bUseTraceResult) ? HitResult.Location : ViewEnd;

	FVector AdjustedAimDir = (AdjustedEnd - TraceStart).GetSafeNormal();
	// TODO: need this check?
	if (AdjustedAimDir.IsZero())
	{
		AdjustedAimDir = ViewDir;
	}

	OutTraceEnd = TraceStart + (AdjustedAimDir * MaxRange);
}

void UWaitForInteractableTarget::LineTrace(FHitResult& OutHitResult, const UWorld* World, const FVector& Start,
	const FVector& End, FName ProfileName, const FCollisionQueryParams Params)
{
	check(World);

	OutHitResult = FHitResult();
	TArray<FHitResult> HitResults;
	World->LineTraceMultiByProfile(HitResults, Start, End, ProfileName, Params);

	OutHitResult.TraceStart = Start;
	OutHitResult.TraceEnd = End;

	if (HitResults.Num() > 0)
	{
		OutHitResult = HitResults[0];
	}
}

void UWaitForInteractableTarget::UpdateInteractableOptions(const FInteractionQuery& InInteractionQuery, const TArray<AActor*>& InteractableTargets)
{
	// TArray<FInteractionOption> NewOptions;
	// InteractableObjectsChanged.Broadcast(NewOptions);
	// TArray<FInteractionOption> TempOptions; TODO:Bug
	// InteractiveTarget->GatherInteractionOptions(InteractQuery, TempOptions);
}

bool UWaitForInteractableTarget::ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection,
                                                             FVector AbilityCenter, float AbilityRange,
                                                             FVector& ClippedPosition)
{
	FVector CameraToCenter = AbilityCenter - CameraLocation;
	float DotToCenter = FVector::DotProduct(CameraToCenter, CameraDirection);
	if (DotToCenter >= 0)		//If this fails, we're pointed away from the center, but we might be inside the sphere and able to find a good exit point.
	{
		float DistanceSquared = CameraToCenter.SizeSquared() - (DotToCenter * DotToCenter);
		float RadiusSquared = (AbilityRange * AbilityRange);
		if (DistanceSquared <= RadiusSquared)
		{
			float DistanceFromCamera = FMath::Sqrt(RadiusSquared - DistanceSquared);
			float DistanceAlongRay = DotToCenter + DistanceFromCamera;						//Subtracting instead of adding will get the other intersection point
			ClippedPosition = CameraLocation + (DistanceAlongRay * CameraDirection);		//Cam aim point clipped to range sphere
			return true;
		}
	}
	return false;
}

void UWaitForInteractableTarget::PerformTrace()
{
	// Ignore actor self
	AActor* AvatarActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
	if (!AvatarActor)
	{
		return;
	}

	UWorld* World = GetWorld();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);

	const bool bTraceComplex = false;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(UAbilityTask_WaitForInteractableTargets_SingleLineTrace), bTraceComplex);
	Params.AddIgnoredActors(ActorsToIgnore);

	// TODO instead of StartLocation I track avatar actor
	FVector TraceStart = AvatarActor->GetActorLocation();
	FVector TraceEnd;
	AimWithPlayerController(AvatarActor, Params, TraceStart, InteractionScanRange, OUT TraceEnd);

	// TODO Trace profile name
	FHitResult OutHitResult;
	LineTrace(OutHitResult, World, TraceStart, TraceEnd, TraceProfile.Name, Params);
	TArray<AActor*> InteractableTargets;
	// Possible multiple hits?
	if (OutHitResult.IsValidBlockingHit())
	{
		InteractableTargets.AddUnique(OutHitResult.GetActor());
	}
	UpdateInteractableOptions(InteractionQuery, InteractableTargets);
	
#if WITH_EDITOR
	if (bShowDebug)
	{
		
		FColor DebugColor = OutHitResult.bBlockingHit ? FColor::Red : FColor::Green;
		if (OutHitResult.bBlockingHit)
		{
			DrawDebugLine(World, TraceStart, OutHitResult.Location, DebugColor, false, InteractionScanRate, 0, 2.0f);
			DrawDebugSphere(World, OutHitResult.Location, 5, 16, DebugColor, false, InteractionScanRate);
		}
		else
		{
			DrawDebugLine(World, TraceStart, TraceEnd, DebugColor, false, InteractionScanRate, 0, 2.0f);
		}
	}
#endif
}
