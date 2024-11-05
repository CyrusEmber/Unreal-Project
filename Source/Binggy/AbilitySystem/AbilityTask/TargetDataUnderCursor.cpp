// Fill out your copyright notice in the Description page of Project Settings.


#include "Binggy/AbilitySystem/AbilityTask/TargetDataUnderCursor.h"

#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

UTargetDataUnderCursor* UTargetDataUnderCursor::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderCursor* MyObj = NewAbilityTask<UTargetDataUnderCursor>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderCursor::UpdateTargetData()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		// Bind target data callback
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(this, &UTargetDataUnderCursor::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(GetAbilitySpecHandle(), GetActivationPredictionKey());
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderCursor::Activate()
{
	UWorld* World = GetWorld();
	UpdateTargetData();
	/*if (World)
	{
		World->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::UpdateTargetData, MeshLocationUpdateRate, true);
	}*/
}

void UTargetDataUnderCursor::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult TraceHitResult;
	TraceUnderCrosshairByVisibility(TraceHitResult);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = TraceHitResult;
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderCursor::TraceUnderCrosshairByVisibility(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);
	
	if (bScreenToWorld) {
		float TraceLength = 80000.f;
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + CrosshairWorldDirection * TraceLength;

		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECC_Visibility);

		if (!TraceHitResult.bBlockingHit) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No Blocking Hit")); 
			TraceHitResult.ImpactPoint = End;
		}
	}
}

void UTargetDataUnderCursor::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
                                                            FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

