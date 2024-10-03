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

	/* Experience Tag */
	FGameplayTag Attributes_Experience_Level;
	FGameplayTag Attributes_Experience_AttributePoints;
	FGameplayTag Attributes_Experience_SkillPoints;

	/* InputTag */
	FGameplayTag InputTag_None;
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	/* Meta Attribute Tag */
	FGameplayTag Damage;
	FGameplayTag Healing;
	FGameplayTag Attributes_Meta_Exp;

	
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

	/* Abilities TODO: None ability */
	FGameplayTag Ability_None;
	FGameplayTag Ability_Fire_FireBall;
	FGameplayTag Ability_Lighting_LightingBall;

	/* Cooldowns */
	FGameplayTag Cooldown_Fire_FireBall;
	FGameplayTag Cooldown_Lighting_LightingBall;

	/* Ability Status Tag */
	FGameplayTag Ability_Status_Locked;
	FGameplayTag Ability_Status_Unlocked;
	FGameplayTag Ability_Status_Equipped;

	/* TODO Ability Type Tag */


	/* Event Tag */
	FGameplayTag GameplayEvent_HitReact;
	FGameplayTag GameplayEvent_Death;


	// TODO usage??
	FGameplayTag GameplayEvent_Attributes_Vigor;
	FGameplayTag GameplayEvent_Attributes_Strength;
	FGameplayTag GameplayEvent_Attributes_Intelligence;
	FGameplayTag GameplayEvent_Attributes_AttributePoint;
	FGameplayTag GameplayEvent_Meta_Exp;




	static FBinggyGameplayTags GameplayTags;

};
