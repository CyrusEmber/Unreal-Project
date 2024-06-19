// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyPlayerState.h"
#include "Binggy/PlayerController/BinggyPlayerController.h"
#include "Binggy/Character/BinggyCharacter.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/AbilitySystem/BinggyAttributeSet.h"
#include "AbilitySystemComponent.h"

ABinggyPlayerState::ABinggyPlayerState()
{
	NetUpdateFrequency = 100.f;

	// Setting gameplay ability system and attribute sets
	AbilitySystemComponent = CreateDefaultSubobject<UBinggyAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UBinggyAttributeSet>("AttributeSet");
}

// This will only execute on clients.
void ABinggyPlayerState::OnRep_Score()
{
	BinggyCharacter = BinggyCharacter == nullptr ? Cast<ABinggyCharacter>(GetPawn()) : BinggyCharacter;
	if (BinggyCharacter) {
		BinggyController = BinggyController == nullptr ? Cast<ABinggyPlayerController>(BinggyCharacter->Controller) : BinggyController;
		if (BinggyController) {
			BinggyController->SetHUDScore(GetScore()); 
		}
	}
}

void ABinggyPlayerState::AddToScore(float ScoreToAdd)
{
	SetScore(GetScore() + ScoreToAdd);
	// For the server
	BinggyCharacter = BinggyCharacter == nullptr ? Cast<ABinggyCharacter>(GetPawn()) : BinggyCharacter;
	if (BinggyCharacter) {
		BinggyController = BinggyController == nullptr ? Cast<ABinggyPlayerController>(BinggyCharacter->Controller) : BinggyController;
		if (BinggyController) {
			BinggyController->SetHUDScore(GetScore());
		}
	}
}

UAbilitySystemComponent* ABinggyPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABinggyPlayerState::BeginPlay()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
	//AddToScore(0);
}
