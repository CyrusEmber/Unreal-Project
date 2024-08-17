// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Singleton 
 */
struct  FBinggyGameplayTags
{
	static const FBinggyGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeGameplayTags();

	//* Primary Attributes where player can allocate points */
	FGameplayTag Attributes_Primary_Vigor;
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;


	//* Secondary Attributes */
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_CriticalDamage;
	FGameplayTag Attributes_Secondary_CriticalChance;
	FGameplayTag Attributes_Secondary_HealthGeneration;
	FGameplayTag Attributes_Secondary_ManaGeneration;

	static FBinggyGameplayTags GameplayTags;

};
