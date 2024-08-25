// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BinggyPlayerController.generated.h"

class AWeapon;
struct FInputActionValue;
class UBinggyInputConfig;
struct FGameplayTag;
class UBinggyAbilitySystemComponent;
class ABinggyHUD;
class UInputMappingContext;
class UInputAction;
class UCombatComponent;

/**
 * 
 */
UCLASS()
class BINGGY_API ABinggyPlayerController : public APlayerController
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
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
	UInputAction* FireAction;

public:
	ABinggyPlayerController();
	
	void SetHUDHealth(float CurrentHealth, float MaxHealth);
	void SetHUDScore(float Score);
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

private:
	ABinggyHUD* BinggyHUD;

	UPROPERTY(VisibleAnywhere)
	UCombatComponent* Combat;

	// Input
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UBinggyInputConfig> InputConfig;
	
	// Inputs
	void Jump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EquipPressed(const FInputActionValue& Value);
	void CrouchPressed(const FInputActionValue& Value);
	void Aim(const FInputActionValue& Value);
	void StopAiming(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);
	void StopFiring(const FInputActionValue& Value);
	void AimOffset(float DeltaTime);

	// More inputs


	float AO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
	
	
};
