// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "BinggyAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UBinggyAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
	
};
