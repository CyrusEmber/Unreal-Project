// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BinggyGameplayAbility.generated.h"

class UBinggyAbilitySystemComponent;
class ABinggyCharacter;
/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Binggy|Input")
	FGameplayTag StartUpInputTag;

	UFUNCTION(BlueprintCallable, Category = "Binggy|Ability")
	ABinggyCharacter* GetBinggyCharacterFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Binggy|Ability")
	UBinggyAbilitySystemComponent* GetBinggyAbilitySystemFromActorInfo() const;

protected:
	
};
