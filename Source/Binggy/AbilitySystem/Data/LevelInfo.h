// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelInfo.generated.h"

USTRUCT(BlueprintType)
struct FBinggyLevelInfo
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 LevelUpRequirement = 0.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 AttributePointAward = 1;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 SpellPointAward = 1;
};

/**
 * 
 */
UCLASS()
class BINGGY_API ULevelInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	int32 GetLevelByXP(int32 XP);

	int32 GetXPByLevel(int32 Level);

private:
	// Index by level, e.g. LevelInformation[1] is level 1 info
	UPROPERTY(EditAnywhere, meta = (TitleProperty = "LevelUpRequirement"))
	TArray<FBinggyLevelInfo> LevelInformation;
	
	
};
