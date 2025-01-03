// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BinggyCharacterBase.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "BinggyEnemy.generated.h"

class UBinggyExperienceSet;
class ABinggyAIController;
class UBehaviorTree;
class UBinggyHealthComponent;
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
	// For AI Controlled character, there is no player state thus directly return AbilitySystemComponent.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Check parent class for more info
	virtual void OnAbilitySystemInitialized() override;
	virtual void OnAbilitySystemUninitialized() override;
	
	virtual void BeginPlay() override;
	// Set the AI controller
	virtual void PossessedBy(AController* NewController) override;

	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Elementalist;



	// AI related
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	// Set from possessedby
	TObjectPtr<ABinggyAIController> AIController;

	// TODO fix the initialization place holder for more attribute sets
	UPROPERTY()
	TObjectPtr<UBinggyExperienceSet> ExperienceSet;
	

private:
	UPROPERTY(VisibleAnywhere, Category = "PlayerState")
	TObjectPtr<UBinggyAbilitySystemComponent> AbilitySystemComponent;
	
};
