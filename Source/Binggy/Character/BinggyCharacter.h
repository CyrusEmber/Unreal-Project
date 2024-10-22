// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BinggyCharacterBase.h"
#include "BinggyCharacter.generated.h"


class UBinggyInventoryComponent;
class UBinggyUIComponent;
class AWeapon;
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
class UPhysicsControlComponent;
UCLASS()
class BINGGY_API ABinggyCharacter : public ABinggyCharacterBase
{
	GENERATED_BODY()

public:
	ABinggyCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	void PlayFiringMontage(bool bAiming);
	void PlayElimMontage();



	// Only execute in the server, since it needs server-authoritative
	virtual void PossessedBy(AController* NewController) override;

	// PlayerState Replication Notification Callback, Init ability actor info for the client
	virtual void OnRep_PlayerState() override;

	// CombatInterface TODO Set it with attribute set
	// virtual int32 GetPlayerLevel() override;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;

	void EquipOverlappingWeapon();

	void FireStart();
	void FireEnd();

	void AimStart();
	void AimEnd();

	// Declare component
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsControlComponent* PhysicsControl;*/
	


protected:
	virtual void BeginPlay() override;
	
	virtual void InitAbilityActorInfo() override;

	virtual void OnAbilitySystemInitialized() override;

	virtual void OnAbilitySystemUninitialized() override;

	// TODO Debug component, todo private assess?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBinggyUIComponent> UIComponent;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBinggyInventoryComponent> InventoryComponent;

	



	

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* OverheadWidget;




	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	// RPC called from client and executed in the server
	UFUNCTION(Server, Reliable)
	void ServerEquip();

	float AO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	//Montage

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* FireWeaponMontage;
	// FIXME
	/*UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;*/
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ElimMontage;

	// TODO? did I used it?
	/*void PlayHitReactMontage();*/

	/*// Elimination Need refactoring
	bool bElimmed = false;
	FTimerHandle ElimTimer;
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 4.f;
	void ElimTimerFinished();*/

public:
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquiped();
	bool IsAiming();
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	// FORCEINLINE bool IsElimmed() const { return bElimmed; }


};
