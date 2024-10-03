// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BinggyWidgetController.h"
#include "GameplayTagContainer.h"
#include "SkillMenuWidgetController.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FBinggyAbilityTags
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();
};

UCLASS(Blueprintable, BlueprintType)
class BINGGY_API USkillMenuWidgetController : public UBinggyWidgetController
{
	GENERATED_BODY()

protected:
	virtual void BroadcastInitialValue() override;
	
	virtual void BindCallbacksToDependencies() override;

	virtual void BeginDestroy() override;

	void BroadcastAbilityInfo(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag) const;

	// TODO: Make it more versatile
	// InAbilityTag is what to put in the equip row. The ReplaceAbilityTag will either have a input tag or it is nullptr. TODO: use struct to handle ability info.
	// Missing a condition that the ability to equip is already equipped, Assume that there is only one ability tag
	UFUNCTION(BlueprintCallable)
	void EquipAbility(const FGameplayTag& AbilityToEquipAbilityTag, const FGameplayTag& EquippedAbilityInputTag);
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	void UnbindAllDelegates();
};
