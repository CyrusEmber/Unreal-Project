// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyGameInstance.h"

#include "Binggy/PlayerController/BinggyPlayerController.h"
#include "Binggy/UI/Subsystem/BinggyUIManagerSubsystem.h"

class UBinggyUIManagerSubsystem;

UBinggyGameInstance::UBinggyGameInstance()
{
}

ABinggyPlayerController* UBinggyGameInstance::GetPrimaryPlayerController() const
{
	return Cast<ABinggyPlayerController>(Super::GetPrimaryPlayerController(false));
}

int32 UBinggyGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int32 ReturnVal = Super::AddLocalPlayer(NewPlayer, UserId);
	if (ReturnVal != INDEX_NONE)
	{
		if (!PrimaryPlayer.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("AddLocalPlayer: Set %s to Primary Player"), *NewPlayer->GetName());
			PrimaryPlayer = NewPlayer;
		}
		
		GetSubsystem<UBinggyUIManagerSubsystem>()->NotifyPlayerAdded(Cast<ULocalPlayer>(NewPlayer));
	}
	
	return ReturnVal;
}

bool UBinggyGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
	if (PrimaryPlayer == ExistingPlayer)
	{
		//TODO: do we want to fall back to another player?
		PrimaryPlayer.Reset();
		UE_LOG(LogTemp, Log, TEXT("RemoveLocalPlayer: Unsetting Primary Player from %s"), *ExistingPlayer->GetName());
	}
	GetSubsystem<UBinggyUIManagerSubsystem>()->NotifyPlayerDestroyed(Cast<ULocalPlayer>(ExistingPlayer));

	return Super::RemoveLocalPlayer(ExistingPlayer);
}

void UBinggyGameInstance::Init()
{
	Super::Init();

	// TODO Create the subsystem for join steam session
}

void UBinggyGameInstance::ReturnToMainMenu()
{
	Super::ReturnToMainMenu();
}

