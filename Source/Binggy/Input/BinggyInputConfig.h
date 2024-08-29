// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "BinggyInputConfig.generated.h"

class UInputAction;
USTRUCT(BlueprintType)
struct FBinggyInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	
	const UInputAction* FindAbilityInputAction(const FGameplayTag InputTag, bool bLogNotFound) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FBinggyInputAction> AbilityInputActions;
	
};
