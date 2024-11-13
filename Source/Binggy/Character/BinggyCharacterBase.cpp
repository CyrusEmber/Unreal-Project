// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "UtilityLibrary.h"
#include "AbilitySystem/BinggyAbilitySystemComponent.h"
#include "AbilitySystem/BinggyGameplayTags.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "GameMode/BinggyGameMode.h"
#include "PlayerController/BinggyPlayerController.h"
#include "PlayerState/BinggyPlayerState.h"
#include "Weapon/Weapon.h"
#include "Component/BinggyHealthComponent.h"
#include "Component/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABinggyCharacterBase::ABinggyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	const FBinggyGameplayTags& GameplayTags = FBinggyGameplayTags::Get();

	// Initialize the component
	HealthComponent = CreateDefaultSubobject<UBinggyHealthComponent>(TEXT("HealthComponent"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	// Niagara components
	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = GameplayTags.Debuff_Burn;

	// Health bar
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());




}

ABinggyPlayerController* ABinggyCharacterBase::GetBinggyPlayerController() const
{
	return Cast<ABinggyPlayerController>(Controller);
}

UBinggyAbilitySystemComponent* ABinggyCharacterBase::GetBinggyAbilitySystemComponent() const
{
	return Cast<UBinggyAbilitySystemComponent>(GetAbilitySystemComponent());
}

ABinggyPlayerState* ABinggyCharacterBase::GetBinggyPlayerState() const
{
	return Cast<ABinggyPlayerState>(GetPlayerState());
}

UAbilitySystemComponent* ABinggyCharacterBase::GetAbilitySystemComponent() const
{
	return GetBinggyPlayerState()->GetAbilitySystemComponent();;
}

void ABinggyCharacterBase::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const bool bHitReact = NewCount > 0;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Hit React Tag Changed")); 
}

void ABinggyCharacterBase::DeathTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Death Tag Changed")); 
}

UAnimMontage* ABinggyCharacterBase::GetHitReactMontage()
{
	check(HitReactMontage);
	return HitReactMontage;
}

void ABinggyCharacterBase::Die(FVector ImpulseDirection, FName BoneName)
{
	// ICombatInterface::Die();

	if (CombatComponent && CombatComponent->EquippedWeapon) {
		CombatComponent->EquippedWeapon->Drop();
	}
	
	MulticastHandleDie(ImpulseDirection, BoneName);

	GetWorldTimerManager().SetTimer(ElimTimer, this, &ThisClass::ElimTimerFinished, ElimDelay);
	
	// TODO: not working
	HealthBar->SetVisibility(false);
	
}

void ABinggyCharacterBase::MulticastHandleDie_Implementation(FVector ImpulseDirection, FName BoneName)
{
	if (GetEquippedWeapon())
	{
		GetEquippedWeapon()->GetWeaponMesh()->SetSimulatePhysics(true);
		GetEquippedWeapon()->GetWeaponMesh()->SetEnableGravity(true);
		GetEquippedWeapon()->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		// GetEquippedWeapon()->GetWeaponMesh()->AddImpulse(ImpulseDirection * 0.5, NAME_None, true);
	}

	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->Stop();
	GetMesh()->bPauseAnims = false;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// Handle ASC TODO: it stops the abilitysystem before the damage take place
	// OnAbilitySystemUninitialized();
	
	// Impulse
	GetMesh()->AddImpulse(ImpulseDirection, BoneName, true);
	this->ForceNetUpdate();
}

void ABinggyCharacterBase::OnAbilitySystemInitialized()
{
	UBinggyAbilitySystemComponent* ASC = GetBinggyAbilitySystemComponent();
	check(ASC);
	
	HealthComponent->InitializeWithAbilitySystem(ASC);
	BurnDebuffComponent->InitializeWithAbilitySystem(ASC);
	// Initialize game tag here with a function TODO:
}

void ABinggyCharacterBase::OnAbilitySystemUninitialized()
{
	HealthComponent->UninitializeFromAbilitySystem();
	BurnDebuffComponent->UninitializeFromAbilitySystem();
}

// Called when the game starts or when spawned
void ABinggyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABinggyCharacterBase::InitAbilityActorInfo()
{
	
}

void ABinggyCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> DefaultAttributes, float level) const
{
	check(DefaultAttributes);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultAttributes, level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ABinggyCharacterBase::InitializeDefaultAttributes() const
{
	// TODO: Placeholder
	ApplyEffectToSelf(DefaultPrimaryAttributes);
	ApplyEffectToSelf(DefaultSecondaryAttributes);
	ApplyEffectToSelf(DefaultVitalAttributes);
}

void ABinggyCharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority())
	{
		return;
	}
	GetBinggyAbilitySystemComponent()->AddCharacterAbilities(StartupAbilities);
	GetBinggyAbilitySystemComponent()->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

FVector ABinggyCharacterBase::GetCombatSocketLocation()
{
	return ICombatInterface::GetCombatSocketLocation();
}

// Called every frame
void ABinggyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABinggyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABinggyCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ABinggyCharacterBase::ElimTimerFinished()
{
	ABinggyGameMode* BinggyGameMode = GetWorld()->GetAuthGameMode<ABinggyGameMode>();
	if (BinggyGameMode) {
		BinggyGameMode->RequestRespawn(this, Controller);
	}
}

AWeapon* ABinggyCharacterBase::GetEquippedWeapon() const
{
	if (CombatComponent == nullptr) {
		return nullptr;
	}
	return CombatComponent->EquippedWeapon;
}

