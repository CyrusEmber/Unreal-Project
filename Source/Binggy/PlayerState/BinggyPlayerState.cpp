// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyPlayerState.h"
#include "PlayerController/BinggyPlayerController.h"
#include "Character/BinggyCharacter.h"
#include "AbilitySystem/BinggyAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/BinggyAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/BinggyExperienceSet.h"
#include "Net/UnrealNetwork.h"

ABinggyPlayerState::ABinggyPlayerState()
{
	NetUpdateFrequency = 100.f;

	// Setting gameplay ability system and attribute sets
	AbilitySystemComponent = CreateDefaultSubobject<UBinggyAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.
	AttributeSet = CreateDefaultSubobject<UBinggyAttributeSet>("AttributeSet");
	ExperienceSet = CreateDefaultSubobject<UBinggyExperienceSet>(TEXT("ExperienceSet"));
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

void ABinggyPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABinggyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABinggyPlayerState, Level);
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

void ABinggyPlayerState::OnRep_Level(int32 OldLevel)
{
}
