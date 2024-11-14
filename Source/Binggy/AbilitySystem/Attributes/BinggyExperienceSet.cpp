// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyExperienceSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

// TODO place holder
UBinggyExperienceSet::UBinggyExperienceSet()
: Experience(100.0f),
Level(1.0f)
{

}

void UBinggyExperienceSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// TODO REPNOTIFY_OnChanged?
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyExperienceSet, Level, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyExperienceSet, Experience, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyExperienceSet, AttributePoints, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyExperienceSet, SkillPoints, COND_None, REPNOTIFY_OnChanged);
}

void UBinggyExperienceSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetInComingExpAttribute())
	{
		SetExperience(GetExperience() + GetInComingExp());
		SetInComingExp(0.f);
	}
	else if (Data.EvaluatedData.Attribute == GetLevelAttribute())
	{
		
	}
}

void UBinggyExperienceSet::OnLevelUp()
{
	// TODO: Fix the coupling problem
	/*const UBinggyAttributeSet* AttributeSet = Cast<UBinggyAbilitySystemComponent>(GetOwningAbilitySystemComponent())->GetSet<UBinggyAttributeSet>();
	AttributeSet->SetHealth()*/
}

void UBinggyExperienceSet::OnRep_Experience(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyExperienceSet, Experience, OldValue);
}

void UBinggyExperienceSet::OnRep_Level(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyExperienceSet, Level, OldValue);
}

void UBinggyExperienceSet::OnRep_AttributePoints(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyExperienceSet, AttributePoints, OldValue);
}

void UBinggyExperienceSet::OnRep_SkillPoints(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyExperienceSet, SkillPoints, OldValue);
}




