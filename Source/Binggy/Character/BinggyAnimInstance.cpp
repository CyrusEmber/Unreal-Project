// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyAnimInstance.h"
#include "BinggyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Binggy/Weapon/Weapon.h"

void UBinggyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	BinggyCharacter = Cast<ABinggyCharacter>(TryGetPawnOwner());
}

void UBinggyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (BinggyCharacter == nullptr) {
		BinggyCharacter = Cast<ABinggyCharacter>(TryGetPawnOwner());
	}
	if (BinggyCharacter == nullptr) {
		return;
	}
	FVector Velocity = BinggyCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
	bIsInAir = BinggyCharacter->GetCharacterMovement()->IsFalling();

	bIsAccelerating = BinggyCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bWeaponEquipped = BinggyCharacter->IsWeaponEquiped();
	EquippedWeapon = BinggyCharacter->GetEquippedWeapon();
	bIsCrouched = BinggyCharacter->bIsCrouched;
	bIsAiming = BinggyCharacter->IsAiming();
	bElimmed = BinggyCharacter->IsElimmed();

	// The rotation of player controller, global rotation
	FRotator AimRotation = BinggyCharacter->GetBaseAimRotation();
	// Also global
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BinggyCharacter->GetVelocity());
	// The difference is Yaw Offset! Both is replicated
	//YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
	YawOffset = DeltaRotation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = BinggyCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);

	AO_Yaw = BinggyCharacter->GetAO_Yaw();
	AO_Pitch = BinggyCharacter->GetAO_Pitch();

	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && BinggyCharacter->GetMesh()) {
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotator;
		BinggyCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(),
			FRotator::ZeroRotator, OutPosition, OutRotator);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotator));
	}
}
