// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
	FString PlayerName;
	if (PlayerState) {
		PlayerName = PlayerState->GetPlayerName();
	}

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Yellow, FString::Printf(TEXT("Total player %i"), NumPlayers));
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, FString::Printf(TEXT("Player %s joined"), *PlayerName));
	}
	// Need fix
	if (NumPlayers == 2) {
		UWorld* World = GetWorld();
		if (World) {
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Maps/MainScene?listen"));
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if (GameState) {
		APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>();
		FString PlayerName;
		if (PlayerState) {
			PlayerName = PlayerState->GetPlayerName();
		}

		if (GEngine) {
			// FIXME 
			GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Yellow, FString::Printf(TEXT("Total player %i"), NumPlayers - 1));
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, FString::Printf(TEXT("Player %s joined"), *PlayerName));
		}
	}
}
