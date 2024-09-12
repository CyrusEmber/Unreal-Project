// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyEnemy.h"

#include "Binggy/UtilityLibrary.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/AbilitySystem/Attributes/BinggyAttributeSet.h"
#include "Component/BinggyHealthComponent.h"
#include "Components/WidgetComponent.h"


ABinggyEnemy::ABinggyEnemy()
{
	bReplicates = true;
	// Setting gameplay ability system and attribute sets
	AbilitySystemComponent = CreateDefaultSubobject<UBinggyAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UBinggyAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

int32 ABinggyEnemy::GetPlayerLevel()
{
	return Level;
}

void ABinggyEnemy::OnAbilitySystemInitialized()
{
	Super::OnAbilitySystemInitialized();
}

void ABinggyEnemy::OnAbilitySystemUninitialized()
{
	Super::OnAbilitySystemUninitialized();
}

void ABinggyEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
}

void ABinggyEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UBinggyAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	InitializeDefaultAttributes();
	OnAbilitySystemInitialized();
	
}

void ABinggyEnemy::InitializeDefaultAttributes() const
{
	UUtilityLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
