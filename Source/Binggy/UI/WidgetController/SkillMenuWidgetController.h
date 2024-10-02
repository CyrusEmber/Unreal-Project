// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BinggyWidgetController.h"
#include "GameplayTagContainer.h"
#include "SkillMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class BINGGY_API USkillMenuWidgetController : public UBinggyWidgetController
{
	GENERATED_BODY()

protected:
	virtual void BroadcastInitialValue() override;
	
	virtual void BindCallbacksToDependencies() override;

	virtual void BeginDestroy() override;

	void BroadcastAbilityInfo(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	void UnbindAllDelegates();
};
