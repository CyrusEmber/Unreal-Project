// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/BinggyDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BinggyGameplayTags.h"

FDamageEffectParams UBinggyDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	// TODO Remove depreciated attribute in the struct
	/*Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());*/
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageTypes = DamageTypes;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebuffFrequency;
	Params.ImpulseMagnitude = ImpulseMagnitude;
	return Params;
}
