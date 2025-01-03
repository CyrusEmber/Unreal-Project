// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"
USTRUCT(BlueprintType)
struct FBinggyAttributeInfo 
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};
/**
 * 
 */
UCLASS()
class BINGGY_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	// Set with data asset
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "AttributeName"))
	TArray<FBinggyAttributeInfo> AttributeInfoArray;

	FBinggyAttributeInfo GetAttributeInfoByTag(FGameplayTag AttributeTag, bool bLogNotFound=false);
};
