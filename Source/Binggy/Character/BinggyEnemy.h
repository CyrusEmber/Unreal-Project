// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BinggyCharacterBase.h"
#include "Binggy/AbilitySystem/Data/CharacterClassInfo.h"
#include "BinggyEnemy.generated.h"

class UWidgetComponent;
enum class ECharacterClass : uint8;
/**
 * 
 */
UCLASS()
class BINGGY_API ABinggyEnemy : public ABinggyCharacterBase
{
	GENERATED_BODY()

public:
	ABinggyEnemy();
	// CombatInterface
	virtual int32 GetPlayerLevel() override;

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Elementalist;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	

private:

	
};
