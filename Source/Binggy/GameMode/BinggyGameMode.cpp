// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyGameMode.h"
#include "Binggy/PlayerController/BinggyPlayerController.h"
#include "Binggy/Character/BinggyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Binggy/PlayerState/BinggyPlayerState.h"


void ABinggyGameMode::BeginPlay()
{
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
	 
}

void ABinggyGameMode::PlayerElimiated(ABinggyCharacter* ElimmedCharacter, ABinggyPlayerController* VictimController, ABinggyPlayerController* AttackerController)
{
	// Set Score
	ABinggyPlayerState* AttackerPlayerState = AttackerController ? Cast<ABinggyPlayerState>(AttackerController->PlayerState) : nullptr;
	ABinggyPlayerState* VictimPlayerState = VictimController ? Cast<ABinggyPlayerState>(VictimController->PlayerState) : nullptr;
	if (AttackerPlayerState && VictimPlayerState && VictimPlayerState != AttackerPlayerState) {
		AttackerPlayerState->AddToScore(1.0f);
	}
	else {
		AttackerPlayerState->AddToScore(-1.0f);
	}
	if (ElimmedCharacter) {
		ElimmedCharacter->Elimination();
	}
}

void ABinggyGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter) {
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController && PlayerStarts.Num() != 0) {
		int32 RandomIndex = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[RandomIndex]);
		
	}
}


