// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyEnemy.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Binggy/UtilityLibrary.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/AbilitySystem/Attributes/BinggyAttributeSet.h"
#include "Binggy/AI/BinggyAIController.h"
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

UAbilitySystemComponent* ABinggyEnemy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// FIXME: TODO: Should enemy have player state?
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

void ABinggyEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AIController = Cast<ABinggyAIController>(NewController);
	if (!HasAuthority()) return;
	AIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AIController->RunBehaviorTree(BehaviorTree);
}

void ABinggyEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UBinggyAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	InitializeDefaultAttributes();
	OnAbilitySystemInitialized();
	
}

// FIXME enemy should have identical attribute and abilities
void ABinggyEnemy::InitializeDefaultAttributes() const
{
	
	UUtilityLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
	UUtilityLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
}
