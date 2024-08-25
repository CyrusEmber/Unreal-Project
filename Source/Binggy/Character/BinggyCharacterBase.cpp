// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"

// Sets default values
ABinggyCharacterBase::ABinggyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

UAbilitySystemComponent* ABinggyCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
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

void ABinggyCharacterBase::InitializeVitalAttributs() const
{
	
}

void ABinggyCharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority())
	{
		return;
	}
	UBinggyAbilitySystemComponent* BinggyASC = CastChecked<UBinggyAbilitySystemComponent>(AbilitySystemComponent);
	BinggyASC->AddCharacterAbilities(StartupAbilities);
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

