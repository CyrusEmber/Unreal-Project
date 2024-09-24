// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BinggyAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DELEGATE_RetVal(FGameplayAttribute, FAttributeSignature);
// TODO: find a way to balance the delegate
DECLARE_MULTICAST_DELEGATE_SixParams(FBinggyAttributeComplexEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec* /*EffectSpec*/, float /*EffectMagnitude*/, float /*OldValue*/, float /*NewValue*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FBinggyAttributeEvent, float /*NewValue*/);
USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FGameplayEffectContextHandle EffectContextHandle;

	// Source
	UAbilitySystemComponent* SourceASC = nullptr;

	AActor* SourceAvatarActor = nullptr;

	AController* SourceController = nullptr;

	ACharacter* SourceCharacter = nullptr;

	// Target
	UAbilitySystemComponent* TargetASC = nullptr;

	AActor* TargetAvatarActor = nullptr;

	AController* TargetController = nullptr;

	ACharacter* TargetCharacter = nullptr;
};

// typedef is specific to the FGameplayAttribute() signature, but TStaticFunPtr is generic to any signature chosen
//typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBinggyAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	// Server Broadcast while OnRep is the client broadcast
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;
	/** Attributes Accessors */
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, Strength);
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, Intelligence);
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, Vigor);
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, MaxMana);
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, HealthRegeneration);
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, ManaRegeneration);

	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, PhysicalDamage);
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, MagicalDamage);
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, CriticalDamage);
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, CriticalChance);
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, Armor);

	// Meta atttribute
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, Healing);



	
	

	/** Primary Attributes */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;


	/** Vital attributes */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category="Attributes")
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Attributes")
	FGameplayAttributeData Mana;
	
	/** Resource attributes */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Attributes")
	FGameplayAttributeData MaxMana;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Attributes")
	FGameplayAttributeData HealthRegeneration;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Attributes")
	FGameplayAttributeData ManaRegeneration;
	
	
	/* Secondary attributes TODO */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Attributes")
	FGameplayAttributeData PhysicalDamage;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Attributes")
	FGameplayAttributeData MagicalDamage;
		
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Attributes")
	FGameplayAttributeData CriticalDamage;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Attributes")
	FGameplayAttributeData CriticalChance;
	
	
	/* Protection Attributes */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Attributes")
	FGameplayAttributeData Armor;
	
	
	/* Meta Attributes */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Damage;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Healing;


	
	
	/** Defining Rep Notify functions */
	UFUNCTION()
	void OnRep_Health(FGameplayAttributeData& OldHealth);
	UFUNCTION()
	void OnRep_MaxHealth(FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_Mana(FGameplayAttributeData& OldMana) const;
	UFUNCTION()
	void OnRep_MaxMana(FGameplayAttributeData& OldMaxMana) const;
	UFUNCTION()
	void OnRep_Strength(FGameplayAttributeData& OldMaxMana) const;
	UFUNCTION()
	void OnRep_Intelligence(FGameplayAttributeData& OldMaxMana) const;
	UFUNCTION()
	void OnRep_Vigor(FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_ManaRegeneration(FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_AttributePoints(FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_SkillPoints(FGameplayAttributeData& OldValue) const;

	// Delegate
	mutable FBinggyAttributeEvent OnHealthChanged;

	mutable FBinggyAttributeEvent OnMaxHealthChanged;

	mutable FBinggyAttributeComplexEvent OnOutOfHealth;

private:
	void SetEffectProperty(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);
	void ShowFloatingText(const FEffectProperties& Props, float InDamage, bool bIsCriticalHit) const;
	// Store the health before any changes 
	float MaxHealthBeforeAttributeChange;
	float HealthBeforeAttributeChange;
	
	// Used to track when the health reaches 0.
	bool bOutOfHealth;

};
