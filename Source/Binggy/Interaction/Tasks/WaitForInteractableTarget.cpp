// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitForInteractableTarget.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Binggy/Interaction/InteractableTarget.h"
#include "Binggy/Interaction/InteractionBlueprintLibrary.h"
#include "Binggy/Interaction/InteractionOption.h"

void UWaitForInteractableTarget::Activate()
{
	SetWaitingOnAvatar();

	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::PerformTrace, InteractionScanRate, true);
	}
	
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
	FCollisionProfileName TraceProfile, FInteractionQuery InInteractionQuery, FGameplayAbilityTargetingLocationInfo
	StartLocation, TSubclassOf<UInterface> TargetInterfaceClass, float InteractionScanRange,
	float InteractionScanRate, bool bShowDebug)
{
	UWaitForInteractableTarget* MyObj = NewAbilityTask<UWaitForInteractableTarget>(OwningAbility);
	MyObj->InteractionScanRange = InteractionScanRange;
	MyObj->InteractionScanRate = InteractionScanRate;
	MyObj->StartLocation = StartLocation;
	MyObj->TraceProfile = TraceProfile;
	MyObj->InteractionQuery = InInteractionQuery;
	MyObj->bShowDebug = bShowDebug;
	MyObj->TargetInterfaceClass = TargetInterfaceClass;

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

void UWaitForInteractableTarget::UpdateInteractableOptions(const FInteractionQuery& InInteractionQuery, const TArray<TScriptInterface<IInteractableTarget>>& InteractableTargets)
{
	TArray<FInteractionOption> NewOptions;
	for (auto InteractiveTarget : InteractableTargets)
	{
		// Check for specific interface
		if (TargetInterfaceClass && !InteractiveTarget.GetObject()->GetClass()->ImplementsInterface(TargetInterfaceClass))
		{
			break;
		}
		TArray<FInteractionOption> TempOptions;
		FInteractionOptionBuilder InteractionBuilder(InteractiveTarget, TempOptions);
		InteractiveTarget->GatherInteractionOptions(InInteractionQuery, InteractionBuilder);
		for (FInteractionOption& Option : TempOptions)
		{
			FGameplayAbilitySpec* InteractionAbilitySpec = nullptr;
			if (Option.InteractionAbilityToGrant)
			{
				
				// TODO: option has target abiltysystem
				if (Option.TargetAbilitySystem && Option.TargetInteractionAbilityHandle.IsValid())
				{
					// Find the spec
					InteractionAbilitySpec = Option.TargetAbilitySystem->FindAbilitySpecFromHandle(Option.TargetInteractionAbilityHandle);
				}
				else if (Option.InteractionAbilityToGrant)
				{
					// Find the spec
					InteractionAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(Option.InteractionAbilityToGrant);

					if (InteractionAbilitySpec)
					{
						// update the option
						Option.TargetAbilitySystem = AbilitySystemComponent.Get();
						Option.TargetInteractionAbilityHandle = InteractionAbilitySpec->Handle;
					}
				}

				if (InteractionAbilitySpec)
				{
					// Filter any options that we can't activate right now for whatever reason.
					if (InteractionAbilitySpec->Ability->CanActivateAbility(InteractionAbilitySpec->Handle, AbilitySystemComponent->AbilityActorInfo.Get()))
					{
						NewOptions.Add(Option);
					}
				}
			}
		}
		
	}
	
	bool bOptionsChanged = false;
	if (NewOptions.Num() == CurrentOptions.Num())
	{
		NewOptions.Sort();

		for (int OptionIndex = 0; OptionIndex < NewOptions.Num(); OptionIndex++)
		{
			const FInteractionOption& NewOption = NewOptions[OptionIndex];
			const FInteractionOption& CurrentOption = CurrentOptions[OptionIndex];

			if (NewOption != CurrentOption)
			{
				bOptionsChanged = true;
				break;
			}
		}
	}
	else
	{
		bOptionsChanged = true;
	}

	if (bOptionsChanged)
	{
		CurrentOptions = NewOptions;
		InteractableObjectsChanged.Broadcast(CurrentOptions);
	}
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
	TArray<TScriptInterface<IInteractableTarget>> InteractableTargets;
	// TODO Possible multiple hits?
	UInteractionBlueprintLibrary::AppendInteractableTargetsFromHitResult(OutHitResult, InteractableTargets);

	// Possible of broadcast empty hit result
	/*FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = OutHitResult;
	DataHandle.Add(Data);*/
	HitResultChanged.Broadcast();
	
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
