// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "BinggyPlayerState.generated.h"

/**
 * 
 */
class ABinggyPlayerController;
class ABinggyCharacter;
class UAbilitySystemComponent;
class UAttributeSet;
UCLASS()
class BINGGY_API ABinggyPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ABinggyPlayerState();
	virtual void OnRep_Score() override;
	void AddToScore(float ScoreToAdd);
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	virtual void BeginPlay() override;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	ABinggyPlayerController* BinggyController;
	ABinggyCharacter* BinggyCharacter;
};
