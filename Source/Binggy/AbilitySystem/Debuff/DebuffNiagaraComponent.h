// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"

class UBinggyAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class BINGGY_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UDebuffNiagaraComponent();

	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag;

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "Binggy|Niagara")
	void InitializeWithAbilitySystem(UBinggyAbilitySystemComponent* InASC);

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "Binggy|Niagara")
	void UninitializeFromAbilitySystem();
	
protected:
	virtual void BeginPlay() override;
	

	void DebuffChanged(const FGameplayTag CallbackTag, int32 NewCount);

	// When the character is dead, deactivate the effect. It's a callback function
	UFUNCTION()
	void HandleOnOutOfHealth(float NewValue);

	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UBinggyAbilitySystemComponent> AbilitySystemComponent;
	
};
