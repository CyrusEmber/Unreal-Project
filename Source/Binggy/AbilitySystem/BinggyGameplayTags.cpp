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

	/* Experience Tag */
	GameplayTags.Attributes_Experience_AttributePoints = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Experience.AttributePoints"));
	GameplayTags.Attributes_Experience_SkillPoints = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Experience.SkillPoints"));
	GameplayTags.Attributes_Experience_Level = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Experience.Level"));
	

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
	
	/* Meta Attribute Tag */
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")
		);
	
	GameplayTags.Healing = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Healing"),
        FString("Healing")
        );

	GameplayTags.Attributes_Meta_Exp = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.XP"),
		FString("Experience change")
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

	/* Abilities */  
	GameplayTags.Ability_Fire_FireBall = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Fire.FireBall"),
	FString("FireBall Ability Tag")
	);
	
	GameplayTags.Ability_Lighting_LightingBall = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Lighting.LightingBall"),
	FString("LightingBall Ability Tag")
	);


	/* Cooldowns */  
	GameplayTags.Cooldown_Fire_FireBall = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Cooldown.Fire.FireBall"),
	FString("FireBall Ability Cooldown")
	);
	
	GameplayTags.Cooldown_Lighting_LightingBall = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Cooldown.Lighting.LightingBall"),
	FString("LightingBall Ability Cooldown")
	);

	/* Ability Status Tag */
	GameplayTags.Ability_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Ability.Status.Locked"),
	FString("The ability is locked either the player has not learned it by getting the ability or not with enough level to unlock it")
	);
	
	GameplayTags.Ability_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Ability.Status.Unlocked"),
	FString("The ability is unlocked and can be used to upgrade or equipped")
	);
	
	GameplayTags.Ability_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Ability.Status.Equipped"),
	FString("Ability Equipped to the current equipped bar")
	);

	/* Gameplay event tag */ 
	GameplayTags.GameplayEvent_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("GameplayEvent.HitReact"),
        FString("Tag grant when hit react")
        );

	GameplayTags.GameplayEvent_Death = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("GameplayEvent.Death"),
        FString("Tag grant when death")
        );

	GameplayTags.GameplayEvent_Attributes_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayEvent.Attributes.Vigor"),
		FString("Vigor change")
		);

	GameplayTags.GameplayEvent_Attributes_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayEvent.Attributes.Strength"),
	FString("Strength change")
	);

	GameplayTags.GameplayEvent_Attributes_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayEvent.Attributes.Intelligence"),
	FString("Intelligence change")
	);

	GameplayTags.GameplayEvent_Attributes_AttributePoint = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayEvent.Attributes.AttributePoint"),
	FString("AttributePoint change")
	);


	
	
}
