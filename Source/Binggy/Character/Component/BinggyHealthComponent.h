// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Components/ActorComponent.h"
#include "BinggyHealthComponent.generated.h"


class UWidgetComponent;
class UBinggyAttributeSet;
class UBinggyAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBinggyHealth_AttributeChanged, float, NewValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BINGGY_API UBinggyHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBinggyHealthComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Returns the health component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "Binggy|Health")
	static UBinggyHealthComponent* FindHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UBinggyHealthComponent>() : nullptr); }

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "Binggy|Health")
	void InitializeWithAbilitySystem(UBinggyAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category = "Binggy|Health")
	void UninitializeFromAbilitySystem();

	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "Binggy|Health")
	float GetHealth() const;

	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "Binggy|Health")
	float GetMaxHealth() const;

	// Returns the current health in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "Binggy|Health")
	float GetHealthNormalized() const;

public:

	// Delegate fired when the health value has changed. This is called on the client but the instigator may not be valid
	UPROPERTY(BlueprintAssignable)
	FBinggyHealth_AttributeChanged OnHealthChanged;

	// Delegate fired when the max health value has changed. This is called on the client but the instigator may not be valid
	UPROPERTY(BlueprintAssignable)
	FBinggyHealth_AttributeChanged OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FBinggyHealth_AttributeChanged OnOutOfHealthChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnUnregister() override;

	// void ClearGameplayTags();

	virtual void HandleHealthChanged(float NewValue);
	virtual void HandleMaxHealthChanged(float NewValue);

	// Called when actor health hit 0
	virtual void HandleOutOfHealth(AActor* EffectInstigator, AActor* DamageCauser, const FGameplayEffectSpec* EffectSpec, float EffectMagnitude, float
	                               OldValue, float NewValue);
	
	// virtual void HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;*/

	// UFUNCTION(BlueprintCallable, Category = "Binggy|Health")
	// UWidgetComponent* GetHealthBar();
protected:

	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UBinggyAbilitySystemComponent> AbilitySystemComponent;

	// Ability set used by this component.
	UPROPERTY()
	TObjectPtr<const UBinggyAttributeSet> AbilitySet;
	

		
};
