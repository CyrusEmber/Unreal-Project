// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyGameplayTags.h"
#include "GameplayTagsManager.h"

// Static instance
FBinggyGameplayTags FBinggyGameplayTags::GameplayTags;

void FBinggyGameplayTags::InitializeNativeGameplayTags()
{
	/* Primary Attributes where player can allocate points */
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"), FString("Increase max health and slightly increase armor"));
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"), FString("Increase max health and slightly increase armor"));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"));


	/* Secondary Attributes */
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"), FString("Reduce Damage"));
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"));
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"));
	GameplayTags.Attributes_Secondary_HealthGeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthGeneration"));
	GameplayTags.Attributes_Secondary_ManaGeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaGeneration"));

	GameplayTags.Attributes_Secondary_CriticalDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.CriticalDamage"));
	GameplayTags.Attributes_Secondary_CriticalChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalChance"));
	GameplayTags.Attributes_Secondary_PhysicalDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.PhysicalDamage"));
	GameplayTags.Attributes_Secondary_MagicalDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MagicalDamage"));

	/* Vital Attributes */
	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Vital.Health"));
	GameplayTags.Attributes_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Vital.Mana"));

	/*
	 * Input Tags
	 */

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button")
		);

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button")
		);

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input Tag for 1 key")
		);

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input Tag for 2 key")
		);

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input Tag for 3 key")
		);

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input Tag for 4 key")
		);

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")
		);
	
	GameplayTags.Healing = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Healing"),
        FString("Healing")
        );
        
    /* Damage Type Tag */
    GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Damage.Fire"),
        FString("Fire Damage Type")
        );

	GameplayTags.Damage_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage.Ice"),
	FString("Ice Damage Type")
	);

	GameplayTags.Damage_Lighting = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage.Lighting"),
	FString("Lighting Damage Type")
	);

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage.Physical"),
	FString("Physical Damage Type")
	);

	/* Resistance Type Tag */
	GameplayTags.Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resistance.Fire"),
		FString("Fire Resistance Type")
		);

	GameplayTags.Resistance_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Resistance.Ice"),
	FString("Ice Resistance Type")
	);

	GameplayTags.Resistance_Lighting = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Resistance.Lighting"),
	FString("Lighting Resistance Type")
	);

	GameplayTags.Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Resistance.Physical"),
	FString("Physical Resistance Type")
	);

      
    /* Add damage types to the map */  
    GameplayTags.DamageTypesToResistanceTypes.Add(GameplayTags.Damage_Fire, GameplayTags.Resistance_Fire);
	GameplayTags.DamageTypesToResistanceTypes.Add(GameplayTags.Damage_Lighting, GameplayTags.Resistance_Lighting);
	GameplayTags.DamageTypesToResistanceTypes.Add(GameplayTags.Damage_Ice, GameplayTags.Resistance_Ice);
	GameplayTags.DamageTypesToResistanceTypes.Add(GameplayTags.Damage_Physical, GameplayTags.Resistance_Physical);

	GameplayTags.GameplayEvent_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("GameplayEvent.HitReact"),
        FString("Tag grant when hit react")
        );

	GameplayTags.GameplayEvent_Death = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("GameplayEvent.Death"),
        FString("Tag grant when death")
        );
	
	
}
