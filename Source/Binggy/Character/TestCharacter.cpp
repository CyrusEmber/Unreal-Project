// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCharacter.h"

#include "Binggy/UtilityLibrary.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/AbilitySystem/Attributes/BinggyAttributeSet.h"
#include "Component/BinggyHealthComponent.h"

ATestCharacter::ATestCharacter()
{
	bReplicates = true;
	// Setting gameplay ability system and attribute sets
	AbilitySystemComponent = CreateDefaultSubobject<UBinggyAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UBinggyAttributeSet>("AttributeSet");
	
}

void ATestCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UBinggyAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	InitializeDefaultAttributes();
	OnAbilitySystemInitialized();
}

void ATestCharacter::OnAbilitySystemInitialized()
{
	Super::OnAbilitySystemInitialized();
	UBinggyAbilitySystemComponent* ASC = Cast<UBinggyAbilitySystemComponent>(GetAbilitySystemComponent());
	check(ASC);
	HealthComponent->InitializeWithAbilitySystem(ASC);
}

void ATestCharacter::InitializeDefaultAttributes() const
{
	Super::InitializeDefaultAttributes();
	UUtilityLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
