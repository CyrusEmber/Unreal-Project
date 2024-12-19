// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildable/Pawn/VehiclePawn.h"
#include "GameFramework/Character.h"

AVehiclePawn::AVehiclePawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVehiclePawn::EnterVehicle(ACharacter* Character)
{
	if (HasAuthority())
	{
		SetOwner(Character);
	}
	
}

void AVehiclePawn::ExitVehicle()
{
	SetOwner(nullptr);
}

void AVehiclePawn::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& OptionBuilder)
{
	OptionBuilder.AddInteractionOption(Option);
}

void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// TODO
// Called to bind functionality to input
void AVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVehiclePawn::Steer(const FInputActionValue& Value)
{
}

void AVehiclePawn::Throttle(const FInputActionValue& Value)
{
}



