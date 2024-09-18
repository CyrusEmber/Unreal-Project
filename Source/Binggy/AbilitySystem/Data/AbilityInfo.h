// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FBinggyAbilityInfo
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> BackgroundMaterial = nullptr;

	// Temporary TODO:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FColor TempColor = FColor();
};

/**
 * 
 */
UCLASS()
class BINGGY_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Information", meta = (TitleProperty = "AbilityTag"))
	TArray<FBinggyAbilityInfo> AbilitiesInformation;

	FBinggyAbilityInfo GetAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false);
	
};
