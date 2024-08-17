// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BinggyWidgetController.h"
#include "Binggy/AbilitySystem/Data/AttributeInfo.h"
#include "AttributeMenuWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoWidgetSigniture, FBinggyAttributeInfo, AttributeInfo);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class BINGGY_API UAttributeMenuWidgetController : public UBinggyWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoWidgetSigniture OnAttributeChanged;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wiget Data")
	TObjectPtr<UDataAsset> AttributeMenuWidgetDataAsset;
	
	
};
