// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "BinggyInventoryItemDefinition.generated.h"

class UBinggyInventoryItemInstance;
// Represents a fragment of an item definition
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class BINGGY_API UBinggyInventoryItemFragment : public UObject
{
	GENERATED_BODY()

public:
	// TODO create something
	virtual void OnInstanceCreated(UBinggyInventoryItemInstance* Instance) const {}
};

/**
 * 
 */
// TODO UCLASS(Blueprintable, Const, Abstract)
UCLASS(Blueprintable)
class BINGGY_API UBinggyInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced)
	TArray<TObjectPtr<UBinggyInventoryItemFragment>> Fragments;

	// The background
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush Background;*/

public:
	const UBinggyInventoryItemFragment* FindFragmentByClass(TSubclassOf<UBinggyInventoryItemFragment> FragmentClass) const;

	// TODO equipment?Description

	// TODO: fragments not the same?
	/*FORCEINLINE bool operator==(const UBinggyInventoryItemDefinition& Other) const
	{
		return DisplayName.IdenticalTo(Other.DisplayName) &&
			Fragments.Num() == Other.Fragments.Num();
	}

	FORCEINLINE bool operator!=(const UBinggyInventoryItemDefinition& Other) const
	{
		return !operator==(Other);
	}*/
};
