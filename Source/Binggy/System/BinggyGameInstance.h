// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BinggyGameInstance.generated.h"

class ABinggyPlayerController;
/**
 * Handle multiplayer session join, Subsystems and Persistent UI Management, @TODO: potentially music system with audio component
 * Each client has a unique copy of game instance throughout the game
 */
UCLASS(Config = Game)
class BINGGY_API UBinggyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UBinggyGameInstance();

	ABinggyPlayerController* GetPrimaryPlayerController() const;
	
	/* TODO: Session join */


	virtual int32 AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId) override;
	virtual bool RemoveLocalPlayer(ULocalPlayer* ExistingPlayer) override;
	virtual void Init() override;
	virtual void ReturnToMainMenu() override;

private:
	/** This is the primary player*/
	TWeakObjectPtr<ULocalPlayer> PrimaryPlayer;
	
};
