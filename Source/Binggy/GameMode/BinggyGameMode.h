// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BinggyGameModeBase.h"
#include "BinggyGameMode.generated.h"

/**
 * 
 */
class ABinggyCharacter;
class ABinggyPlayerController;
class ACharacter;
UCLASS()
class BINGGY_API ABinggyGameMode : public ABinggyGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PlayerElimiated(ABinggyCharacter* ElimmedCharacter, ABinggyPlayerController* VictimController, ABinggyPlayerController* AttackerController);
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);

	

protected:
	virtual void BeginPlay() override;
	TArray<AActor*> PlayerStarts;

	
};
