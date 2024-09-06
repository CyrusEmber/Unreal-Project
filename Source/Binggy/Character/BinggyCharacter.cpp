// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Binggy/Weapon/Weapon.h"
#include "Binggy/Character/Component/CombatComponent.h"
#include "Binggy/Binggy.h"
#include "Binggy/PlayerController/BinggyPlayerController.h"
#include "Binggy/GameMode/BinggyGameMode.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "TimerManager.h"
#include "Binggy/PlayerState/BinggyPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Binggy/UtilityLibrary.h"
#include "Binggy/AbilitySystem/BinggyGameplayTags.h"
#include "Binggy/UI/HUD/BinggyHUD.h"


// Sets default values
ABinggyCharacter::ABinggyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	//// Set size for collision capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	//// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	//// instead of recompiling to adjust them
	//GetCharacterMovement()->JumpZVelocity = 700.f;
	//GetCharacterMovement()->AirControl = 0.35f;
	//GetCharacterMovement()->MaxWalkSpeed = 500.f;
	//GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	//GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	//GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);
	OverheadWidget->CanCharacterStepUpOn = ECB_No;

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	// This is a component, only need to set replicated like this
	Combat->SetIsReplicated(true);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	// Net 
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
}

void ABinggyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABinggyCharacter, OverlappingWeapon, COND_OwnerOnly);
	DOREPLIFETIME(ABinggyCharacter, Health);
}

void ABinggyCharacter::EquipOverlappingWeapon()
{
	if (OverlappingWeapon) {
		if (HasAuthority()) {
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else {
			ServerEquip();
		}
	}
}

void ABinggyCharacter::FireStart()
{
	if (Combat) {
		Combat->FirePressed(true);
	}
}

void ABinggyCharacter::FireEnd()
{
	if (Combat) {
		Combat->FirePressed(false);
	}
}

void ABinggyCharacter::AimStart()
{
	if (Combat) {
		Combat->SetAiming(true);
	}
}

void ABinggyCharacter::AimEnd()
{
	if (Combat) {
		Combat->SetAiming(false);
	}
}

void ABinggyCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateHUDHealth();

	// Damage is calculated on the server
	if (HasAuthority()) {
		OnTakeAnyDamage.AddDynamic(this, &ThisClass::ReceiveDamage);
	}
	
}

void ABinggyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABinggyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat) {
		Combat->Character = this;
	}
}

void ABinggyCharacter::PlayFiringMontage(bool bAiming)
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) {
		return;
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireWeaponMontage) {
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName = FName("Rifle_Hip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABinggyCharacter::PlayElimMontage()
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) {
		return;
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ElimMontage) {
		AnimInstance->Montage_Play(ElimMontage);
	}
}

void ABinggyCharacter::Elimination()
{
	if (Combat && Combat->EquippedWeapon) {
		Combat->EquippedWeapon->Drop();
	}
	MulticastElimination();
	GetWorldTimerManager().SetTimer(ElimTimer, this, &ThisClass::ElimTimerFinished, ElimDelay);


	// Avoid collision
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABinggyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Init ability actor info for the server, set ability system component
	InitAbilityActorInfo();
	AddCharacterAbilities();
	
	// TODO: Refactoring
	AbilitySystemComponent->RegisterGameplayTagEvent(FBinggyGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
		this, &ThisClass::HitReactTagChanged
	);

	// Fixme this is not working
	UUtilityLibrary::GiveStartupAbilities(this, GetAbilitySystemComponent());

}

void ABinggyCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Init ability actor info for the client
	InitAbilityActorInfo();
}

int32 ABinggyCharacter::GetPlayerLevel()
{
	ABinggyPlayerState* BinggyPlayerState = GetPlayerState<ABinggyPlayerState>();
	return BinggyPlayerState->GetPlayerLevel();
}

// TODO: Potentially check the usage of ASC or AS
void ABinggyCharacter::InitAbilityActorInfo()
{
	ABinggyPlayerState* BinggyPlayerState = GetPlayerState<ABinggyPlayerState>();
	check(BinggyPlayerState);
	BinggyPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(BinggyPlayerState, this);
	Cast<UBinggyAbilitySystemComponent>(BinggyPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	AbilitySystemComponent = BinggyPlayerState->GetAbilitySystemComponent();
	AttributeSet = BinggyPlayerState->GetAttributeSet();
	BinggyPlayerController = BinggyPlayerController == nullptr ? Cast<ABinggyPlayerController>(Controller) : BinggyPlayerController;
	if (BinggyPlayerController) {
		if (ABinggyHUD* BinggyHUD = Cast<ABinggyHUD>(BinggyPlayerController->GetHUD())) {
			BinggyHUD->InitOverlay(BinggyPlayerController);
		}
	}

	InitializeDefaultAttributes();
}

void ABinggyCharacter::MulticastElimination_Implementation()
{
	bElimmed = true;
	PlayElimMontage();

	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	if (BinggyPlayerController) {
		DisableInput(BinggyPlayerController);
	}

	// Stop any running animations
	//if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	//{
	//	AnimInstance->StopAllMontages(1.f); // Adjust blend out time as needed
	//}

	// Enable physics on the skeletal mesh
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	//// Disable hit event
	////GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->bBlendPhysics = true;

	// Optionally, apply an impulse to the ragdoll
	//FVector Impulse = FVector(0, 0, 200.0f);
	//GetMesh()->AddImpulseToAllBodiesBelow(Impulse, GetMesh()->GetBoneName(0), true);

	// Enable ragdoll
	//if (BinggyPlayerController) {
	//	DisableInput(BinggyPlayerController);
	//}
}

void ABinggyCharacter::ElimTimerFinished()
{
	ABinggyGameMode* BinggyGameMode = GetWorld()->GetAuthGameMode<ABinggyGameMode>();
	if (BinggyGameMode) {
		BinggyGameMode->RequestRespawn(this, Controller);
	}
}


void ABinggyCharacter::PlayHitReactMontage()
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) {
		return;
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && GetHitReactMontage()) {
		AnimInstance->Montage_Play(GetHitReactMontage());
		FName SectionName = FName("Default");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

// TODO
void ABinggyCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon) {
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeapon) {
		LastWeapon->ShowPickupWidget(false);
	}
}

void ABinggyCharacter::ServerEquip_Implementation()
{
	if (OverlappingWeapon && HasAuthority()) {
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

// Only on server
void ABinggyCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	PlayHitReactMontage();
	UpdateHUDHealth();
	if (Health == 0.f) {
		ABinggyGameMode* BinggyGameMode = GetWorld()->GetAuthGameMode<ABinggyGameMode>();
		if (BinggyGameMode) {

			BinggyPlayerController = BinggyPlayerController == nullptr ? Cast<ABinggyPlayerController>(Controller) : BinggyPlayerController;
			ABinggyPlayerController* AttackerPlayerController = Cast<ABinggyPlayerController>(InstigatorController);
			// Check null in gamemode
			BinggyGameMode->PlayerElimiated(this, BinggyPlayerController, AttackerPlayerController);
		}
		
	}
	
}

void ABinggyCharacter::OnRep_Health()
{
	PlayHitReactMontage();
	UpdateHUDHealth();
}


void ABinggyCharacter::UpdateHUDHealth()
{
	BinggyPlayerController = BinggyPlayerController == nullptr ? Cast<ABinggyPlayerController>(Controller) : BinggyPlayerController;
	if (BinggyPlayerController) {
		BinggyPlayerController->SetHUDHealth(Health, MaxHealth);
	}
}

void ABinggyCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	// No need to check since only the local player can see the widget, so broadcast to all the client still working
	if (OverlappingWeapon) {
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = Weapon;
	if (IsLocallyControlled()) {
		if (OverlappingWeapon) {
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

bool ABinggyCharacter::IsWeaponEquiped()
{
	return (Combat && Combat->EquippedWeapon);
}

bool ABinggyCharacter::IsAiming()
{
	return (Combat && Combat->bIsAiming);
}

AWeapon* ABinggyCharacter::GetEquippedWeapon()
{
	if (Combat == nullptr) {
		return nullptr;
	}
	return Combat->EquippedWeapon;
}






