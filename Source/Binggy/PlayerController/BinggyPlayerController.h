// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Buildable/Pawn/VehiclePawn.h"
#include "GameFramework/PlayerController.h"
#include "BinggyPlayerController.generated.h"

class UDamageTextWidgetComponent;
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
public:
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	// Debug Action. now activate the build menu
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DebugAction;

	// Build mode context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Build", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* BuildModeMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Build", meta = (AllowPrivateAccess = "true"))
	UInputAction* SpinAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Build", meta = (AllowPrivateAccess = "true"))
	UInputAction* ZoomAction;

	// TODO: make it soft ptr?
	// Vehicle mode context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Vehicle", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* VehicleModeMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Build", meta = (AllowPrivateAccess = "true"))
	UInputAction* SteerAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Build", meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrottleAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Build", meta = (AllowPrivateAccess = "true"))
	UInputAction* BreakAction;


public:
	ABinggyPlayerController();
	
	void SetHUDScore(float Score);
	
	virtual void OnPossess(APawn* InPawn) override;
	// Client acknowledge, initialize client HUD
	virtual void AcknowledgePossession(APawn* P) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void SwitchBuildMode(bool bIsBuildMode, TSubclassOf<UCommonActivatableWidget> BuildModeWidgetClass);

	// Possess only execute in the server, but input context binding only execute locally.
	UFUNCTION(BlueprintCallable)
	void SwitchVehicleMode(bool bIsVehicleMode, AVehiclePawn* Vehicle);

	// Is it the good position to place it?
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bIsCriticalHit);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextWidgetComponent> DamageTextComponentClass;

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

	TObjectPtr<UBinggyAbilitySystemComponent> BinggyAbilitySystemComponent;

	UBinggyAbilitySystemComponent* GetASC();
	
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
	void InteractPressed(const FInputActionValue& Value);
	void AimOffset(float DeltaTime);
	
	// More inputs
	
	float AO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
	
	
};
