// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BinggyAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class BINGGY_API ABinggyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABinggyAIController();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
