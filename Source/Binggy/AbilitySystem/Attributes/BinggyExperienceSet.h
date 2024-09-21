// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BinggyExperienceSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class UBinggyAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyExperienceSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBinggyExperienceSet();

	ATTRIBUTE_ACCESSORS(UBinggyExperienceSet, Experience);
	ATTRIBUTE_ACCESSORS(UBinggyExperienceSet, Level);
	ATTRIBUTE_ACCESSORS(UBinggyExperienceSet, InComingExp);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Server Broadcast while OnRep is the client broadcast
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	
protected:
	UFUNCTION()
	void OnRep_Experience(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Level(const FGameplayAttributeData& OldValue);
	

private:
	// The current experience attribute.  The health will be capped by the max health attribute.  Health is hidden from modifiers so only executions can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Experience, Category = "Lyra|Experience", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Experience;

	// The current max health attribute.  Max health is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Level, Category = "Lyra|Experience", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Level;

	// Meta attribute
	UPROPERTY(BlueprintReadOnly, Category = "Lyra|Experience", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData InComingExp;
	
};
