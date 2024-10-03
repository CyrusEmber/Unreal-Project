// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityLibrary.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/FBinggyGameplayEffectContext.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Binggy/UI/HUD/BinggyHUD.h"
#include "GameMode/BinggyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/BinggyPlayerState.h"

UOverlayWidgetController* UUtilityLibrary::GetOverlayWidgetController(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (ABinggyHUD* HUD = Cast<ABinggyHUD>(PlayerController->GetHUD()))
		{
			return HUD->GetOverlayWidgetController();
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UUtilityLibrary::GetAttributeMenuWidgetController(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (ABinggyHUD* HUD = Cast<ABinggyHUD>(PlayerController->GetHUD()))
		{
			return HUD->GetAttributeMenuController();
		}
	}
	return nullptr;
}

USkillMenuWidgetController* UUtilityLibrary::GetSkillMenuWidgetController(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (ABinggyHUD* HUD = Cast<ABinggyHUD>(PlayerController->GetHUD()))
		{
			return HUD->GetSkillMenuController();
		}
	}
	return nullptr;
}

UAbilityInfo* UUtilityLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const ABinggyGameModeBase* BinggyGameMode = Cast<ABinggyGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (BinggyGameMode != nullptr)
	{
		return BinggyGameMode->AbilityInfoDataAsset;
	}
	return nullptr;
}

void UUtilityLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass,
                                                  float Level, UAbilitySystemComponent* ASC)
{
	const ABinggyGameModeBase* BinggyGameMode = Cast<ABinggyGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (BinggyGameMode == nullptr)
	{
		return;
	}

	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = BinggyGameMode->CharacterClassInfo;

	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	// Apply the gameplay effect with source object
	ApplyAttributes(ASC, Level, AvatarActor, ClassDefaultInfo.PrimaryAttributes);
	ApplyAttributes(ASC, Level, AvatarActor, CharacterClassInfo->SecondaryAttributes);
	ApplyAttributes(ASC, Level, AvatarActor, CharacterClassInfo->VitalAttributes);
}

void UUtilityLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	check(ASC);
	ABinggyGameModeBase* BinggyGameMode = Cast<ABinggyGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (BinggyGameMode == nullptr)
	{
		return;
	}
	check(BinggyGameMode);
	UCharacterClassInfo* CharacterClassInfo = BinggyGameMode->CharacterClassInfo;
	check(CharacterClassInfo);
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	
}

bool UUtilityLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBinggyGameplayEffectContext* BinggyEffectContext = static_cast<const FBinggyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BinggyEffectContext->IsCriticalHit();
	}
	return false;
}

void UUtilityLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FBinggyGameplayEffectContext* BinggyEffectContext = static_cast<FBinggyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BinggyEffectContext->SetCriticalHit(bInIsCriticalHit);
	}
}

FGameplayTag UUtilityLibrary::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	check(&AbilitySpec);
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UUtilityLibrary::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	check(&AbilitySpec);
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UUtilityLibrary::GetAbilityStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	check(&AbilitySpec);
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability.Status"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

void UUtilityLibrary::ApplyAttributes(UAbilitySystemComponent* ASC, float Level, AActor* AvatarActor, TSubclassOf<UGameplayEffect> Attributes)
{
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(Attributes, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());
}
