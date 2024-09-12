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
#include "Component/BinggyHealthComponent.h"


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

	/*CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	// This is a component, only need to set replicated like this
	CombatComponent->SetIsReplicated(true);*/

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	// Net 
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

	// Health
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	HealthBar->SetOwnerNoSee(true);  // Hide for the owner

}

void ABinggyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABinggyCharacter, OverlappingWeapon, COND_OwnerOnly);
}

void ABinggyCharacter::EquipOverlappingWeapon()
{
	if (OverlappingWeapon) {
		if (HasAuthority()) {
			CombatComponent->EquipWeapon(OverlappingWeapon);
		}
		else {
			ServerEquip();
		}
	}
}

void ABinggyCharacter::FireStart()
{
	if (CombatComponent) {
		CombatComponent->FirePressed(true);
	}
}

void ABinggyCharacter::FireEnd()
{
	if (CombatComponent) {
		CombatComponent->FirePressed(false);
	}
}

void ABinggyCharacter::AimStart()
{
	if (CombatComponent) {
		CombatComponent->SetAiming(true);
	}
}

void ABinggyCharacter::AimEnd()
{
	if (CombatComponent) {
		CombatComponent->SetAiming(false);
	}
}

void ABinggyCharacter::Die()
{
	Super::Die();
	if (GetEquippedWeapon())
	{
		GetEquippedWeapon()->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
		GetEquippedWeapon()->GetWeaponMesh()->SetSimulatePhysics(true);
		GetMesh()->SetEnableGravity(true);
		GetEquippedWeapon()->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->Stop();
	GetMesh()->bPauseAnims = false;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABinggyCharacter::BeginPlay()
{
	Super::BeginPlay();

    // Hide health bar for local user
	if (IsLocallyControlled())
	{
		HealthBar->SetVisibility(false);
	}
	
}

void ABinggyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABinggyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CombatComponent) {
		CombatComponent->Character = this;
	}
}

void ABinggyCharacter::PlayFiringMontage(bool bAiming)
{
	if (CombatComponent == nullptr || CombatComponent->EquippedWeapon == nullptr) {
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
	if (CombatComponent == nullptr || CombatComponent->EquippedWeapon == nullptr) {
		return;
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ElimMontage) {
		AnimInstance->Montage_Play(ElimMontage);
	}
}

void ABinggyCharacter::Elimination()
{
	if (CombatComponent && CombatComponent->EquippedWeapon) {
		CombatComponent->EquippedWeapon->Drop();
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
	UBinggyAbilitySystemComponent* AbilitySystemComponent = GetBinggyAbilitySystemComponent();
	// Fixme this is not working
	UUtilityLibrary::GiveStartupAbilities(this, GetAbilitySystemComponent());
	// TODO: Refactoring
	AbilitySystemComponent->RegisterGameplayTagEvent(FBinggyGameplayTags::Get().GameplayEvent_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
		this, &ThisClass::HitReactTagChanged
	);

	AbilitySystemComponent->RegisterGameplayTagEvent(FBinggyGameplayTags::Get().GameplayEvent_Death, EGameplayTagEventType::NewOrRemoved).AddUObject(
	this, &ThisClass::DeathTagChanged
	);
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
	ABinggyPlayerState* BinggyPlayerState = GetBinggyPlayerState();
	check(BinggyPlayerState);
	UBinggyAbilitySystemComponent* AbilitySystemComponent = GetBinggyAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(BinggyPlayerState, this);
	AbilitySystemComponent->AbilityActorInfoSet();

	
	AttributeSet = BinggyPlayerState->GetAttributeSet();
	if (ABinggyPlayerController* BinggyPlayerController = GetBinggyPlayerController()) {
		if (ABinggyHUD* BinggyHUD = Cast<ABinggyHUD>(BinggyPlayerController->GetHUD())) {
			BinggyHUD->InitOverlay(BinggyPlayerController);
		}
	}

	InitializeDefaultAttributes();

	// Initialize the health component
	HealthComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
	// TODO: Refactor
	/*if (GetController()->IsLocalPlayerController())
	{
		HealthBar->SetVisibility(false, true);
	}*/
}

void ABinggyCharacter::MulticastElimination_Implementation()
{
	bElimmed = true;
	PlayElimMontage();

	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	if (ABinggyPlayerController* BinggyPlayerController = GetBinggyPlayerController()) {
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
	if (CombatComponent == nullptr || CombatComponent->EquippedWeapon == nullptr) {
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
		CombatComponent->EquipWeapon(OverlappingWeapon);
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
	return (CombatComponent && CombatComponent->EquippedWeapon);
}

bool ABinggyCharacter::IsAiming()
{
	return (CombatComponent && CombatComponent->bIsAiming);
}

AWeapon* ABinggyCharacter::GetEquippedWeapon()
{
	if (CombatComponent == nullptr) {
		return nullptr;
	}
	return CombatComponent->EquippedWeapon;
}






