// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AWeapon;
class ABinggyCharacter;
class ABinggyPlayerController;
class ABinggyHUD;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BINGGY_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	friend ABinggyCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Only called in the server with reliable RPC from client. Because we want this to be replicated through all the client
	void EquipWeapon(AWeapon* WeaponToEquipped);

protected:
	virtual void BeginPlay() override;

public:
	void SetAiming(bool bAiming);
	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bAiming);

	void FirePressed(bool bPressed);

	void Fire();

	UFUNCTION()
	void OnRep_EquippedWeapon();

	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

	// FIXME public
public:
	ABinggyCharacter* Character;
	ABinggyPlayerController* Controller;
	ABinggyHUD* HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(Replicated)
	bool bIsAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;
	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;

	// Crosshair factors
	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootFactor;

	// Aiming and FOV

	float DefaultFOV;

	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 30.f;

	float CurrentFOV;

	float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);

	// Automatic fire

	FTimerHandle FireTimer;

	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();
	
	
public:	
	
	

		
};
