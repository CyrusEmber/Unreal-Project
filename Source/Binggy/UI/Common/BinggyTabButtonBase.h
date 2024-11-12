// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BinggyTabListWidgetBase.h"
#include "Binggy/UI/Foundation/BinggyButtonBase.h"
#include "BinggyTabButtonBase.generated.h"

/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyTabButtonBase : public UBinggyButtonBase, public IBinggyTabButtonInterface
{
	GENERATED_BODY()

public:

	/*void SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject);
	void SetIconBrush(const FSlateBrush& Brush);*/

protected:

	UFUNCTION()
	virtual void SetTabLabelInfo_Implementation(const FBinggyTabDescriptor& TabLabelInfo) override;

private:

	/*UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonLazyImage> LazyImage_Icon;*/
	
};
