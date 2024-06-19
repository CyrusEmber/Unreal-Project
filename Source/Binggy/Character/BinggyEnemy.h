// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BinggyCharacterBase.h"
#include "BinggyEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BINGGY_API ABinggyEnemy : public ABinggyCharacterBase
{
	GENERATED_BODY()

public:
	ABinggyEnemy();
protected:
	virtual void BeginPlay() override;
	
};
