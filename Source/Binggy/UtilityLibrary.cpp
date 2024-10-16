// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/BinggyGameplayTags.h"
#include "AbilitySystem/FBinggyGameplayEffectContext.h"
#include "AbilitySystem/Abilities/BinggyAbilityTypes.h"
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

bool UUtilityLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBinggyGameplayEffectContext* BinggyEffectContext = static_cast<const FBinggyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BinggyEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UUtilityLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBinggyGameplayEffectContext* BinggyEffectContext = static_cast<const FBinggyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BinggyEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UUtilityLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBinggyGameplayEffectContext* BinggyEffectContext = static_cast<const FBinggyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BinggyEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UUtilityLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBinggyGameplayEffectContext* BinggyEffectContext = static_cast<const FBinggyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BinggyEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UUtilityLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBinggyGameplayEffectContext* BinggyEffectContext = static_cast<const FBinggyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (BinggyEffectContext->GetDamageType().IsValid())
		{
			return *BinggyEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UUtilityLibrary::GetImpulseDirection(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBinggyGameplayEffectContext* AuraEffectContext = static_cast<const FBinggyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetImpulseDirection();
	}
	return FVector::ZeroVector;
}

void UUtilityLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FBinggyGameplayEffectContext* BinggyEffectContext = static_cast<FBinggyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BinggyEffectContext->SetCriticalHit(bInIsCriticalHit);
	}
}

void UUtilityLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulDebuff)
{
	if (FBinggyGameplayEffectContext* BinggyEffectContext = static_cast<FBinggyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BinggyEffectContext->SetIsSuccessfulDebuff(bInSuccessfulDebuff);
	}
}

void UUtilityLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FBinggyGameplayEffectContext* BinggyEffectContext = static_cast<FBinggyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BinggyEffectContext->SetDebuffDamage(InDamage);
	}
}

void UUtilityLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDuration)
{
	if (FBinggyGameplayEffectContext* BinggyEffectContext = static_cast<FBinggyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BinggyEffectContext->SetDebuffDuration(InDuration);
	}
}

void UUtilityLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InFrequency)
{
	if (FBinggyGameplayEffectContext* BinggyEffectContext = static_cast<FBinggyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BinggyEffectContext->SetDebuffFrequency(InFrequency);
	}
}

void UUtilityLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FBinggyGameplayEffectContext* BinggyEffectContext = static_cast<FBinggyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		BinggyEffectContext->SetDamageType(DamageType);
	}
}

void UUtilityLibrary::SetImpulseDirection(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InImpulseDirection)
{
	if (FBinggyGameplayEffectContext* BinggyEffectContext = static_cast<FBinggyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BinggyEffectContext->SetImpulseDirection(InImpulseDirection);
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

FGameplayEffectContextHandle UUtilityLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FBinggyGameplayTags& GameplayTags = FBinggyGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContexthandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContexthandle.AddSourceObject(SourceAvatarActor);
	SetImpulseDirection(EffectContexthandle, DamageEffectParams.ImpulseDirection);
	EffectContexthandle.AddHitResult(DamageEffectParams.HitResult);
	
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContexthandle);
	for (auto& Pair : DamageEffectParams.DamageTypes)
	{
		// GetAbilityLevel() TODO
		const float ScaledDamage = Pair.Value.GetValueAtLevel(1.f);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContexthandle;
}

void UUtilityLibrary::ApplyAttributes(UAbilitySystemComponent* ASC, float Level, AActor* AvatarActor, TSubclassOf<UGameplayEffect> Attributes)
{
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(Attributes, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());
}
