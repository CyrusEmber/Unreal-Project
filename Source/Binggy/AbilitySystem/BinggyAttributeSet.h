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
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;


	/** Primary Attributes */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, Strength);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, Intelligence);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, Vigor);


	/** Vital attributes */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category="Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, Health);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, Mana);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, MaxHealth);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, MaxMana);
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, HealthRegeneration);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Attributes")
	FGameplayAttributeData MenaRegeneration;
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, MenaRegeneration);

	/* Secondary attributes */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes")
	FGameplayAttributeData PhysicalDamage;
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, PhysicalDamage);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes")
	FGameplayAttributeData MagicalDamage;
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, MagicalDamage);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Attributes")
	FGameplayAttributeData CriticalDamage;
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, CriticalDamage);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Attributes")
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, CriticalChance);

	/* Protection Attributes */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthGeneration, Category = "Attributes")
	FGameplayAttributeData HealthGeneration;
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, HealthGeneration);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaGeneration, Category = "Attributes")
	FGameplayAttributeData ManaGeneration;
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, ManaGeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaGeneration, Category = "Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UBinggyAttributeSet, Armor);

	
	/** Defining Rep Notify functions */
	UFUNCTION()
	void OnRep_Health(FGameplayAttributeData& OldHealth) const;
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
	void OnRep_HealthGeneration(FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_ManaGeneration(FGameplayAttributeData& OldValue) const;

private:
	void SetEffectProperty(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);


};
