// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyAttributeSet.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Binggy/AbilitySystem/BinggyGameplayTags.h"
#include "GameFramework/Character.h"

UBinggyAttributeSet::UBinggyAttributeSet()
{
	FBinggyGameplayTags GameplayTags = FBinggyGameplayTags::Get();
	/* Primary */
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	/* Secondary */
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthGeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaGeneration, GetManaRegenerationAttribute);

	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalDamage, GetCriticalDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalChance, GetCriticalChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_PhysicalDamage, GetPhysicalDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MagicalDamage, GetMagicalDamageAttribute);
}

void UBinggyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Respond to the change even if the value is unchanged
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

// Although the value is clamped, it still changed somehow
void UBinggyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}

}

void UBinggyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties Props;
	SetEffectProperty(Data, Props);


	// Ensure Health and Mana do not go below 0 or above their max values
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Hit Location: %f"), GetHealth()));
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
	else if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Character: %s, Health: %f"), *Props.TargetAvatarActor->GetName(),GetHealth()));
			const bool bFatal = NewHealth <= 0.f;
			

			if (!bFatal)
			{
				FGameplayTagContainer TagContainer;
	            TagContainer.AddTag(FBinggyGameplayTags::Get().Effects_HitReact);
	            Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
		}
	}

	
}

void UBinggyAttributeSet::SetEffectProperty(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid()) {
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr) {
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor)) {
				Props.SourceController = Pawn->GetController();
			}
		}
		// Bugged FIXME
		//if (Props.SourceController) {
		//	ACharacter* SourceCharacter1 = Cast<ACharacter>(Props.SourceController->GetPawn());
		//}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()) {
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}

}

void UBinggyAttributeSet::OnRep_Health(FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyAttributeSet, Health, OldHealth);
}

void UBinggyAttributeSet::OnRep_MaxHealth(FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyAttributeSet, MaxHealth, OldMaxHealth);
}

void UBinggyAttributeSet::OnRep_Mana(FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyAttributeSet, Mana, OldMana);
}

void UBinggyAttributeSet::OnRep_MaxMana(FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyAttributeSet, MaxMana, OldMaxMana);
}

void UBinggyAttributeSet::OnRep_Strength(FGameplayAttributeData& OldMaxMana) const
{
}

void UBinggyAttributeSet::OnRep_Intelligence(FGameplayAttributeData& OldMaxMana) const
{
}

void UBinggyAttributeSet::OnRep_Vigor(FGameplayAttributeData& OldMaxMana) const
{
}

void UBinggyAttributeSet::OnRep_HealthRegeneration(FGameplayAttributeData& OldValue) const
{
}

void UBinggyAttributeSet::OnRep_ManaRegeneration(FGameplayAttributeData& OldValue) const
{
}


