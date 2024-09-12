// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Binggy/AbilitySystem/Data/CharacterClassInfo.h"
#include "Binggy/Character/BinggyCharacterBase.h"
#include "TestCharacter.generated.h"

class UBinggyHealthComponent;
/**
 * 
 */
UCLASS()
class BINGGY_API ATestCharacter : public ABinggyCharacterBase
{
	GENERATED_BODY()

public:
	ATestCharacter();

protected:
	virtual void InitAbilityActorInfo() override;
	virtual void OnAbilitySystemInitialized() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Elementalist;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	int32 Level = 1;

private:
	
};
