// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Singleton class that stores all the gameplay tags
 */
struct  FBinggyGameplayTags
{
	static const FBinggyGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeGameplayTags();

	/* Primary Attributes where player can allocate points */
	FGameplayTag Attributes_Primary_Vigor;
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;


	/* Secondary Attributes */
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_HealthGeneration;
	FGameplayTag Attributes_Secondary_ManaGeneration;
	
	FGameplayTag Attributes_Secondary_CriticalDamage;
	FGameplayTag Attributes_Secondary_CriticalChance;
	FGameplayTag Attributes_Secondary_PhysicalDamage;
	FGameplayTag Attributes_Secondary_MagicalDamage;

	/* Vital Attributes */
	FGameplayTag Attributes_Vital_Health;
	FGameplayTag Attributes_Vital_Mana;

	/* InputTag */
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	/* Meta Attribute Tag */
	FGameplayTag Damage;
	FGameplayTag Healing;
	
	/* Damage Type Tag */
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Ice;
	FGameplayTag Damage_Lighting;
	FGameplayTag Damage_Physical;

	/* Resistance Type Tag */
	FGameplayTag Resistance_Fire;
	FGameplayTag Resistance_Ice;
	FGameplayTag Resistance_Lighting;
	FGameplayTag Resistance_Physical;
	
	/* Damage to resistance mapping */
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistanceTypes;

	/* Event Tag */
	FGameplayTag GameplayEvent_HitReact;
	FGameplayTag GameplayEvent_Death;
	

	static FBinggyGameplayTags GameplayTags;

};
