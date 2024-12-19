// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CommonActivatableWidget.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Character/Component/CombatComponent.h"
#include "UI/HUD/BinggyHUD.h"
#include "Character/BinggyCharacter.h"
#include "PlayerState/BinggyPlayerState.h"
#include "Input/BinggyInputComponent.h"
#include "UI/Widget/DamageTextWidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/Subsystem/BinggyUIManagerSubsystem.h"
#include "NativeGameplayTags.h"
#include "Buildable/Pawn/VehiclePawn.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_GAME, "UI.Layer.Game");

ABinggyPlayerController::ABinggyPlayerController()
{
	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	// This is a component, only need to set replicated like this
	Combat->SetIsReplicated(true);
	// TODO: why?
	bReplicates = true;

}

void ABinggyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(DefaultMappingContext);
	if (!BinggyHUD)
	{
		BinggyHUD = Cast<ABinggyHUD>(GetHUD());
	}
	

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	
}

void ABinggyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// ABinggyCharacter* BinggyCharacter = Cast<ABinggyCharacter>(InPawn);

	ABinggyPlayerState* BinggyPlayerState = Cast<ABinggyPlayerState>(PlayerState);
	if (BinggyPlayerState) {
		BinggyPlayerState->AddToScore(0);
	}

	// Initialize HUD for server
	if (!BinggyHUD) {
		BinggyHUD = Cast<ABinggyHUD>(GetHUD());
	}
	
	if (IsLocalController() && IsValid(Cast<ABinggyCharacterBase>(this->GetPawn())))
	{
		BinggyHUD->InitOverlay(Cast<ABinggyCharacterBase>(this->GetPawn())->GetBinggyAbilitySystemComponent());
	}
}

void ABinggyPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);
	if (IsLocalController())
	{
		// Initialize HUD for client, TODO vehicle HUD
		if (BinggyHUD) {
			// Make sure the player character is character, not vehicle.
			if (Cast<ABinggyCharacterBase>(this->GetPawn()))
			{
				BinggyHUD->InitOverlay(Cast<ABinggyCharacterBase>(this->GetPawn())->GetBinggyAbilitySystemComponent());
			}
			
		}
	}
}

void ABinggyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimOffset(DeltaTime);
}

void ABinggyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABinggyPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABinggyPlayerController::StopJump);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABinggyPlayerController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABinggyPlayerController::Look);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ABinggyPlayerController::EquipPressed);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ABinggyPlayerController::CrouchPressed);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ABinggyPlayerController::Aim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ABinggyPlayerController::StopAiming);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ABinggyPlayerController::InteractPressed);
		// TODO
		/*EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ABinggyPlayerController::Fire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ABinggyPlayerController::StopFiring);*/
	}
	
	// TODO: Refactoring
	if (UBinggyInputComponent* BinggyInputComponent = Cast<UBinggyInputComponent>(InputComponent))
	{
		BinggyInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
}

void ABinggyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ABinggyPlayerController::SwitchBuildMode(bool bIsBuildMode, TSubclassOf<UCommonActivatableWidget> BuildModeWidgetClass)
{
	if (bIsBuildMode)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BuildModeMappingContext, 1);
		}
		// TODO: set the UI for build mode
		UUIBlueprintLibrary::PushWidgetToLayerStack(this, TAG_UI_LAYER_GAME, BuildModeWidgetClass);
	} else
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(BuildModeMappingContext);
		}
		// TODO Deactivate the widget
	}
}

void ABinggyPlayerController::SwitchVehicleMode(bool bIsVehicleMode,
	AVehiclePawn* Vehicle)
{
	if (bIsVehicleMode)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->RemoveMappingContext(DefaultMappingContext);
			}
			Subsystem->AddMappingContext(BuildModeMappingContext, 0);
		}
		GetCharacter()->AttachToActor(Vehicle, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		// TODO check if the vehicle has owner.
		Vehicle->SetOwner(this->GetPawn());
		Possess(Vehicle);
	} else
	{ // Return to the original mapping.
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
			Subsystem->RemoveMappingContext(BuildModeMappingContext);
		}
		// TODO: Make sure the owner is pawn.
		Vehicle->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		Possess(Cast<APawn>(Vehicle->GetOwner()));
		Vehicle->SetOwner(nullptr);
	}
}

void ABinggyPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bIsCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextWidgetComponent* DamageText = NewObject<UDamageTextWidgetComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageTextAndColor(DamageAmount, bIsCriticalHit);
	}
}

void ABinggyPlayerController::SetHUDScore(float Score)
{
	//BinggyHUD = BinggyHUD == nullptr ? Cast<ABinggyHUD>(GetHUD()) : BinggyHUD;
	//bool bHUDValid = BinggyHUD && BinggyHUD->CharacterOverlay && BinggyHUD->CharacterOverlay->ScoreAmount;
	////GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
	//if (bHUDValid) {
	//	FString ScoreText = FString::Printf(TEXT("%d"), FMath::CeilToInt(Score));
	//	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, ScoreText);
	//	BinggyHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
	//}
}

void ABinggyPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, *InputTag.ToString());
}

void ABinggyPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagHeld(InputTag);
}

void ABinggyPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

UBinggyAbilitySystemComponent* ABinggyPlayerController::GetASC()
{
	if (BinggyAbilitySystemComponent == nullptr)
	{
		BinggyAbilitySystemComponent = Cast<UBinggyAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return BinggyAbilitySystemComponent;
}

void ABinggyPlayerController::Jump(const FInputActionValue& Value)
{
	if (ABinggyCharacter* ControlledCharacter = Cast<ABinggyCharacter>(GetCharacter()))
	{
		ControlledCharacter->Jump();
	}
}

void ABinggyPlayerController::StopJump(const FInputActionValue& Value)
{
	if (ABinggyCharacter* ControlledCharacter = Cast<ABinggyCharacter>(GetCharacter()))
	{
		ControlledCharacter->StopJumping();
	}
}

void ABinggyPlayerController::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	if (ABinggyCharacter* ControlledCharacter = Cast<ABinggyCharacter>(GetPawn()))
	{	ControlledCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledCharacter->AddMovementInput(RightDirection, MovementVector.X);
	}
	
}

// TODO change in settings
void ABinggyPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (ABinggyCharacter* ControlledCharacter = Cast<ABinggyCharacter>(GetPawn()))
	{
		// add yaw and pitch input to controller
		ControlledCharacter->AddControllerYawInput(0.2 * LookAxisVector.X);
		ControlledCharacter->AddControllerPitchInput(0.2 * LookAxisVector.Y);
	}

	else if (AVehiclePawn* ControlledVehicle = Cast<AVehiclePawn>(GetPawn()))
	{
		// add yaw and pitch input to controller
		Cast<ABinggyCharacter>(ControlledVehicle->GetOwner())->AddControllerYawInput(0.2 * LookAxisVector.X);
		Cast<ABinggyCharacter>(ControlledVehicle->GetOwner())->AddControllerPitchInput(0.2 * LookAxisVector.Y);
	}
}

// Equip the weapon on server if on clients
void ABinggyPlayerController::EquipPressed(const FInputActionValue& Value)
{	
	if (ABinggyCharacter* ControlledCharacter = Cast<ABinggyCharacter>(GetPawn()))
	{
		ControlledCharacter->EquipOverlappingWeapon();
	}
}

void ABinggyPlayerController::CrouchPressed(const FInputActionValue& Value)
{
	if (ABinggyCharacter* ControlledCharacter = Cast<ABinggyCharacter>(GetPawn()))
	{
		if (ControlledCharacter->bIsCrouched) {
			ControlledCharacter->UnCrouch();
		}
		else {
			ControlledCharacter->Crouch();
		}
	}
	
}

void ABinggyPlayerController::Aim(const FInputActionValue& Value)
{
	if (ABinggyCharacter* ControlledCharacter = Cast<ABinggyCharacter>(GetPawn()))
	{
		ControlledCharacter->AimStart();
	}
}

void ABinggyPlayerController::StopAiming(const FInputActionValue& Value)
{
	if (ABinggyCharacter* ControlledCharacter = Cast<ABinggyCharacter>(GetPawn()))
	{
		ControlledCharacter->AimEnd();
	}
}

void ABinggyPlayerController::Fire(const FInputActionValue& Value)
{
	if (ABinggyCharacter* ControlledCharacter = Cast<ABinggyCharacter>(GetPawn()))
	{
		ControlledCharacter->FireStart();
	}
}

void ABinggyPlayerController::StopFiring(const FInputActionValue& Value)
{
	if (ABinggyCharacter* ControlledCharacter = Cast<ABinggyCharacter>(GetPawn()))
	{
		ControlledCharacter->FireEnd();
	}
}

// TODO: the delegate broadcasts interact ability
void ABinggyPlayerController::InteractPressed(const FInputActionValue& Value)
{
	
}

void ABinggyPlayerController::AimOffset(float DeltaTime)
{
	if (Combat && Combat->EquippedWeapon == nullptr) {
		return;
	}

	if (ABinggyCharacter* ControlledCharacter = Cast<ABinggyCharacter>(GetPawn()))
	{
		FVector Velocity = ControlledCharacter->GetVelocity();
		Velocity.Z = 0.f;
		float Speed = Velocity.Size();

		bool bIsInAir = ControlledCharacter->GetCharacterMovement()->IsFalling();

		if (Speed == 0.f && !bIsInAir) {
			FRotator CurrretAimRotation = FRotator(0.f, ControlledCharacter->GetBaseAimRotation().Yaw, 0.f);
			FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrretAimRotation, ControlledCharacter->GetActorRotation());
			AO_Yaw = DeltaAimRotation.Yaw;
			ControlledCharacter->bUseControllerRotationYaw = false;
		}
		// Could potentially be problematic because the base aim rotation not the character rotation
		if (Speed != 0.f || bIsInAir) {
			AO_Yaw = 0.f;
			ControlledCharacter->bUseControllerRotationYaw = true;
		}
		// Its being compressed when sending to the network
		AO_Pitch = ControlledCharacter->GetBaseAimRotation().Pitch;
		if (AO_Pitch > 90.f && !ControlledCharacter->IsLocallyControlled()) {
			AO_Pitch = AO_Pitch - 360.f;
		}
	}
}


