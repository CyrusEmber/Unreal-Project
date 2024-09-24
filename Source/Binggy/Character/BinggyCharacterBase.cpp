// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Binggy/UtilityLibrary.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/AbilitySystem/BinggyGameplayTags.h"
#include "Binggy/PlayerController/BinggyPlayerController.h"
#include "Binggy/PlayerState/BinggyPlayerState.h"
#include "Binggy/Weapon/Weapon.h"
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

	// Initialize the component
	HealthComponent = CreateDefaultSubobject<UBinggyHealthComponent>(TEXT("HealthComponent"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	// Health bar
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

}

ABinggyPlayerController* ABinggyCharacterBase::GetBinggyPlayerController() const
{
	return CastChecked<ABinggyPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

UBinggyAbilitySystemComponent* ABinggyCharacterBase::GetBinggyAbilitySystemComponent() const
{
	return Cast<UBinggyAbilitySystemComponent>(GetAbilitySystemComponent());
}

ABinggyPlayerState* ABinggyCharacterBase::GetBinggyPlayerState() const
{
	return CastChecked<ABinggyPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
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

void ABinggyCharacterBase::Die()
{
	// ICombatInterface::Die();

	if (CombatComponent && CombatComponent->EquippedWeapon) {
		CombatComponent->EquippedWeapon->Drop();
	}
	
	MulticastHandleDie();
	
	// TODO: not working
	HealthBar->SetVisibility(false);
	
}

void ABinggyCharacterBase::MulticastHandleDie_Implementation()
{
	if (GetEquippedWeapon())
	{
		GetEquippedWeapon()->GetWeaponMesh()->SetSimulatePhysics(true);
		GetMesh()->SetEnableGravity(true);
		GetEquippedWeapon()->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
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
	

	
	this->ForceNetUpdate();
}

void ABinggyCharacterBase::OnAbilitySystemInitialized()
{
	UBinggyAbilitySystemComponent* ASC = GetBinggyAbilitySystemComponent();
	check(ASC);
	
	HealthComponent->InitializeWithAbilitySystem(ASC);
	// Initialize game tag here with a function TODO:
}

void ABinggyCharacterBase::OnAbilitySystemUninitialized()
{
	HealthComponent->UninitializeFromAbilitySystem();
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

AWeapon* ABinggyCharacterBase::GetEquippedWeapon() const
{
	if (CombatComponent == nullptr) {
		return nullptr;
	}
	return CombatComponent->EquippedWeapon;
}

