// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
 
#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Interface/CombatInterface.h"
#include "GameFramework/Character.h"
#include "BinggyCharacterBase.generated.h"


class UDebuffNiagaraComponent;
class UWidgetComponent;
class UExperienceComponent;
class AWeapon;
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnASCSet);

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
	virtual void Die(FVector ImpulseDirection, FName BoneName) override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDie(FVector ImpulseDirection, FName BoneName);

	FORCEINLINE virtual float GetKilledExperience() const override { return KilledExperience; }


protected:
	// Initialize the component when ability system component set, calls GetBinggyAbilitySystemComponent which will be overriden in AI controlled classes.
	virtual void OnAbilitySystemInitialized();
	// Uninitialize the component's connection with ASC.
	virtual void OnAbilitySystemUninitialized();

	// Actor Interface?
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	// It is different for player and AI, since player store the ASC in player state
	virtual void InitAbilityActorInfo();

	// Save a copy of ASC and AS to the character. ASC is only stored in the Enemy class
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UExperienceComponent> ExperienceComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;

	// Widget
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> DamageTextWidgetClass;

	// In world Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	// Weapon
	virtual FVector GetCombatSocketLocation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Experience")
	float KilledExperience = 10.f;

private:	
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	// Elimination Need refactoring
	bool bElimmed = false;
	FTimerHandle ElimTimer;
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 4.f;
	void ElimTimerFinished();

public:
	// TODO: put it into static function
	FORCEINLINE UCombatComponent* GetCombatComponent() const { return CombatComponent; }
	AWeapon* GetEquippedWeapon() const;

};
