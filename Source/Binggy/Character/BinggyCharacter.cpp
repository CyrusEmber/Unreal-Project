// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
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
#include "Binggy/AbilitySystem/Attributes/BinggyExperienceSet.h"
#include "Binggy/UI/HUD/BinggyHUD.h"
#include "Component/BinggyHealthComponent.h"
#include "Component/BinggyUIComponent.h"
#include "Component/ExperienceComponent.h"
#include "Components/CapsuleComponent.h"


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

	RootComponent = GetCapsuleComponent();

	// Create the Physics Control Component
	/*PhysicsControl = CreateDefaultSubobject<UPhysicsControlComponent>(TEXT("PhysicsControl"));
	PhysicsControl->SetupAttachment(RootComponent);	*/


	// Camera Component TODO
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

	// Set collision channel

	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_HitBox);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// Net 
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
	
	// Initialize the component
	ExperienceComponent = CreateDefaultSubobject<UExperienceComponent>(TEXT("ExperienceComponent"));
	UIComponent = CreateDefaultSubobject<UBinggyUIComponent>(TEXT("UIComponent"));
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
	// GetWorldTimerManager().SetTimer(ElimTimer, this, &ThisClass::ElimTimerFinished, ElimDelay);


	// Avoid collision
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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



void ABinggyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Init ability actor info for the server, set ability system component
	// Make sure add ability first then initialize the overlay
	AddCharacterAbilities();
	InitAbilityActorInfo();
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
	UBinggyAbilitySystemComponent* AbilitySystemComponent = Cast<UBinggyAbilitySystemComponent>(BinggyPlayerState->GetAbilitySystemComponent());

	if (AbilitySystemComponent->GetAvatarActor() != this)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Not equal")));
	}
	
	AbilitySystemComponent->InitAbilityActorInfo(BinggyPlayerState, this);

	// Debug
	/*UGameplayEffect* GEDebug = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Bounty")));
	GEDebug->DurationPolicy = EGameplayEffectDurationType::Instant;

	int32 Idx = GEDebug->Modifiers.Num();
	GEDebug->Modifiers.SetNum(Idx + 1);

	FGameplayModifierInfo& AttributePoints = GEDebug->Modifiers[Idx];
	AttributePoints.ModifierMagnitude = FScalableFloat(1.0f);
	AttributePoints.ModifierOp = EGameplayModOp::Additive;
	AttributePoints.Attribute = UBinggyExperienceSet::GetAttributePointsAttribute();*/

	// AbilitySystemComponent->ApplyGameplayEffectToSelf(GEDebug, 1.0f, AbilitySystemComponent->MakeEffectContext());
	//AbilitySystemComponent->AbilityActorInfoSet();

	/*
	if (!HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Client Character Avatar name: %s"), *this->GetName()));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Client Character ASC name: %s"), *AbilitySystemComponent->GetAvatarActor()->GetName()));
	}
	*/
	InitializeDefaultAttributes();

	// Initialize the health and EXP component for all clients and server, for server, you need to initialize component for server and clients, for clients, initialize component for server and clients

	// Only the server abilitysystem component can 
	/*if (HasAuthority())
	UIComponent->InitializeWithAbilitySystem(AbilitySystemComponent);*/
	
	
	
	
	// AttributeSet = BinggyPlayerState->GetAttributeSet();
	// Initialize the HUD with PS, however, the client ability system component version , should initial overlay with server version ASC
	ABinggyPlayerController* BinggyPlayerController = GetBinggyPlayerController();
	if (BinggyPlayerController) {
            if (ABinggyHUD* BinggyHUD = Cast<ABinggyHUD>(BinggyPlayerController->GetHUD())) {
            	// UIComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
                BinggyHUD->InitOverlay(AbilitySystemComponent);
            	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Init Character ASC name: %s"), *AbilitySystemComponent->GetOwner()->GetName()));
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Init Character ASC name: %s"), *AbilitySystemComponent->GetAvatarActor()->GetName()));*/
            }
	}
	// The health component and experience component can broadcast initial value to overlay
	HealthComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
	ExperienceComponent->InitializeWithAbilitySystem(AbilitySystemComponent);


	
	

	

	// TODO: Refactor
	/*if (GetController()->IsLocalPlayerController())
	{
		HealthBar->SetVisibility(false, true);
	}*/
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








