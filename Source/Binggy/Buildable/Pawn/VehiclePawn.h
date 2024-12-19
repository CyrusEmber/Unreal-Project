// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interaction/InteractableTarget.h"
#include "VehiclePawn.generated.h"

UCLASS()
class BINGGY_API AVehiclePawn : public APawn, public IInteractableTarget
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVehiclePawn();

	void EnterVehicle(ACharacter* Character);

	void ExitVehicle();

	// Steer the wheels that have steering motor.
	UFUNCTION(BlueprintImplementableEvent)
	void Steer();
	
	UFUNCTION(BlueprintImplementableEvent)
	// Power the wheels that have a throttle motor.
	void Throttle();

	// Interaction
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Default")
	FInteractionOption Option;
	

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	// TODO: make it soft ptr?
	// Vehicle mode context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Vehicle", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* VehicleModeMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Build", meta = (AllowPrivateAccess = "true"))
	UInputAction* SteerAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Build", meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrottleAction;
	// TODO?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Build", meta = (AllowPrivateAccess = "true"))
	UInputAction* BreakAction;

	// Inputs Binding Throttle
	void Steer(const FInputActionValue& Value);
	void Throttle(const FInputActionValue& Value);

};
