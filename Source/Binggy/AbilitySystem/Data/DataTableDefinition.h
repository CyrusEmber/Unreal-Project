// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DataTableDefinition.generated.h"

class UNiagaraComponent;
class UGameplayEffect;

// Find according things for ability
USTRUCT(BlueprintType)
struct FAbilityRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AbilityTag;

	// TODO FDataTableRowHandle curve table
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float  Cost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float  Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float  Damage;

	// TODO: this is created dynamically now
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EffectToSelf;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EffectToTarget;

	// TODO: Where to initialize the effect?
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UNiagaraComponent> VisualEffect;
};


