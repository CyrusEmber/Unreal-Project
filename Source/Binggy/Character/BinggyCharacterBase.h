// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
 
#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Binggy/Interface/CombatInterface.h"
#include "GameFramework/Character.h"
#include "BinggyCharacterBase.generated.h"


class ABinggyPlayerState;
class UBinggyAbilitySystemComponent;
class ABinggyPlayerController;
class UCombatComponent;
class UBinggyHealthComponent;
struct FGameplayTag;
class UGameplayAbility;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;

// Default class for AI or player character, it has a health component and a combat component. It also has some common functions for a character
UCLASS(Abstract)
class BINGGY_API ABinggyCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ABinggyCharacterBase();
	UFUNCTION(BlueprintCallable, Category = "Character")
	ABinggyPlayerController* GetBinggyPlayerController() const;
	
	UFUNCTION(BlueprintCallable, Category = "Character")
	UBinggyAbilitySystemComponent* GetBinggyAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Character")
	ABinggyPlayerState* GetBinggyPlayerState() const;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	void DeathTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* GetHitReactMontage() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void Die() override;

protected:
	// Initialize the component when ability system component set, calls GetBinggyAbilitySystemComponent which will be overriden in AI controlled classes.
	virtual void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemUninitialized();

	// Actor Interface?
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	// It is different for player and AI, since player store the ASC in player state
	virtual void InitAbilityActorInfo();

	// Save a copy of ASC and AS to the character
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	
	// Attributes
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	// Set after primary attributes
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	
	// Apply any attribute effect to self
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> DefaultAttributes, float level = 1) const;
	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbilities();

	// Common components of character TODO: Move it into private section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBinggyHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCombatComponent> CombatComponent;

	// Weapon
	virtual FVector GetCombatSocketLocation();

private:	
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

};
