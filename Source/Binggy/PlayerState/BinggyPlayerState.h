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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// Getter
	FORCEINLINE int32 GetPlayerLevel() { return Level; }

protected:
	virtual void BeginPlay() override;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	// Experience attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UBinggyExperienceSet> ExperienceSet;

private:
	ABinggyPlayerController* BinggyController;
	ABinggyCharacter* BinggyCharacter;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
};
