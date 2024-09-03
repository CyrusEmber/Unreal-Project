// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BinggyGameplayAbility.generated.h"

class ABinggyCharacter;
/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartUpInputTag;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	ABinggyCharacter* GetBinggyCharacterFromActorInfo() const;
protected:
	
};
