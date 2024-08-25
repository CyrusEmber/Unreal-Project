// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h" 
#include "BinggyCharacterBase.h"
#include "BinggyCharacter.generated.h"


class UBinggyAbilitySystemComponent;
struct FGameplayTag;
class UBinggyInputConfig;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UWidgetComponent;
class UCombatComponent;
struct FInputActionValue;
class UAnimMontage;
class ABinggyPlayerController;
class AController;
class UAbilitySystemComponent;
UCLASS()
class BINGGY_API ABinggyCharacter : public ABinggyCharacterBase
{
	GENERATED_BODY()

	/** MappingContext */
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;*/

public:
	ABinggyCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	void PlayFiringMontage(bool bAiming);
	void PlayElimMontage();
	// Only on server
	void Elimination();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastElimination();

	// AbilitySystem
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// CombatInterface
	virtual int32 GetPlayerLevel() override;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	void EquipOverlappingWeapon();

	void FireStart();
	void FireEnd();

	void AimStart();
	void AimEnd();
	
	

protected:
	virtual void BeginPlay() override;

	

	// Inputs
	/*void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EquipPressed(const FInputActionValue& Value);
	void CrouchPressed(const FInputActionValue& Value);
	void Aim(const FInputActionValue& Value);
	void StopAiming(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);
	void StopFiring(const FInputActionValue& Value);
	void AimOffset(float DeltaTime);*/
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser);
	void UpdateHUDHealth();

	virtual void InitAbilityActorInfo() override;

	

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* OverheadWidget;



	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UPROPERTY(VisibleAnywhere)
	UCombatComponent* Combat;

	// RPC called from client and executed in the server
	UFUNCTION(Server, Reliable)
	void ServerEquip();

	float AO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	//Montage

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* FireWeaponMontage;
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ElimMontage;

	void PlayHitReactMontage();

	// Player Health
	UPROPERTY(EditAnywhere, Category="Player Stats")
	float MaxHealth = 100.f;
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health = 100.f;

	UFUNCTION()
	void OnRep_Health();

	ABinggyPlayerController* BinggyPlayerController;

	bool bElimmed = false;

	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 4.f;

	void ElimTimerFinished();
	
	// Input
	/*void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UBinggyInputConfig> InputConfig;*/

	/*UPROPERTY()
	TObjectPtr<UBinggyAbilitySystemComponent> BinggyAbilitySystemComponent;*/
	


public:
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquiped();
	bool IsAiming();
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	AWeapon* GetEquippedWeapon();

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool IsElimmed() const { return bElimmed; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
};
