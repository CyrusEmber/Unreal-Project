// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyPlayerController.h"
#include "Binggy/UI/HUD/BinggyHUD.h"
#include "Binggy/UI/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Binggy/Character/BinggyCharacter.h"
#include "Binggy/PlayerState/BinggyPlayerState.h"

void ABinggyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	BinggyHUD = Cast<ABinggyHUD>(GetHUD());
}

void ABinggyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ABinggyCharacter* BinggyCharacter = Cast<ABinggyCharacter>(InPawn);

	if (BinggyCharacter) {
		SetHUDHealth(BinggyCharacter->GetHealth(), BinggyCharacter->GetMaxHealth());
	}

	ABinggyPlayerState* BinggyPlayerState = Cast<ABinggyPlayerState>(PlayerState);
	if (BinggyPlayerState) {
		BinggyPlayerState->AddToScore(0);
	}
}

void ABinggyPlayerController::SetHUDHealth(float CurrentHealth, float MaxHealth)
{
	//BinggyHUD = BinggyHUD == nullptr ? Cast<ABinggyHUD>(GetHUD()) : BinggyHUD;
	//bool bHUDValid = BinggyHUD && BinggyHUD->CharacterOverlay && BinggyHUD->CharacterOverlay->HealthBar && BinggyHUD->CharacterOverlay->HealthText;
	//if (bHUDValid) {
	//	const float HealthPercent = CurrentHealth / MaxHealth;
	//	BinggyHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
	//	FString HealthText = FString::Printf(TEXT("%d"), FMath::CeilToInt(CurrentHealth));
	//	BinggyHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	//}
}

void ABinggyPlayerController::SetHUDScore(float Score)
{
	//BinggyHUD = BinggyHUD == nullptr ? Cast<ABinggyHUD>(GetHUD()) : BinggyHUD;
	//bool bHUDValid = BinggyHUD && BinggyHUD->CharacterOverlay && BinggyHUD->CharacterOverlay->ScoreAmount;
	////GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
	//if (bHUDValid) {
	//	FString ScoreText = FString::Printf(TEXT("%d"), FMath::CeilToInt(Score));
	//	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, ScoreText);
	//	BinggyHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
	//}
}


