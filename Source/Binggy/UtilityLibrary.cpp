// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityLibrary.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Binggy/UI/HUD/BinggyHUD.h"
#include "GameMode/BinggyGameModeBase.h"
#include "Kismet/GameplayStatics.h"

UOverlayWidgetController* UUtilityLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ABinggyHUD* HUD = Cast<ABinggyHUD>(PC->GetHUD()))
		{
			return HUD->GetOverlayWidgetController(PC);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UUtilityLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ABinggyHUD* HUD = Cast<ABinggyHUD>(PC->GetHUD()))
		{
			return HUD->GetAttributeMenuController(PC);
		}
	}
	return nullptr;
}

void UUtilityLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass,
	float Level, UAbilitySystemComponent* ASC)
{
	ABinggyGameModeBase* BinggyGameMode = Cast<ABinggyGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (BinggyGameMode == nullptr) return;

	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = BinggyGameMode->CharacterClassInfo;
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	// Apply the gameplay effect with source object
	ApplyAttributes(ASC, Level, AvatarActor, ClassDefaultInfo.PrimaryAttributes);
	ApplyAttributes(ASC, Level, AvatarActor, CharacterClassInfo->SecondaryAttributes);
	ApplyAttributes(ASC, Level, AvatarActor, CharacterClassInfo->VitalAttributes);
}

void UUtilityLibrary::ApplyAttributes(UAbilitySystemComponent* ASC, float Level, AActor* AvatarActor, TSubclassOf<UGameplayEffect> Attributes)
{
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(Attributes, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());
}
