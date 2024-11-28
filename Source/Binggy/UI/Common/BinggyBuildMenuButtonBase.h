// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/Foundation/BinggyButtonBase.h"
#include "BinggyBuildMenuButtonBase.generated.h"

class UBuildableDefinition;
class ABinggyWorldBuildable;
struct FBuildablePlaceHolder;
/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyBuildMenuButtonBase : public UBinggyButtonBase
{
	GENERATED_BODY()

public:
	UBinggyBuildMenuButtonBase();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default", meta = (ExposeOnSpawn = "true"))
	TSubclassOf<UBuildableDefinition> BuildableDef;

protected:
	// Blueprint implemented event
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonIcon(const UTexture2D* InTexture);
	
	
};
