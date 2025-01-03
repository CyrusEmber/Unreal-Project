// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyEnemy.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UtilityLibrary.h"
#include "AbilitySystem/BinggyAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/BinggyAttributeSet.h"
#include "AbilitySystem/Attributes/BinggyExperienceSet.h"
#include "AI/BinggyAIController.h"


ABinggyEnemy::ABinggyEnemy()
{
	bReplicates = true;
	// Setting gameplay ability system and attribute sets
	AbilitySystemComponent = CreateDefaultSubobject<UBinggyAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UBinggyAttributeSet>("AttributeSet");
	ExperienceSet = CreateDefaultSubobject<UBinggyExperienceSet>("ExperienceSet");


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

	check(AbilitySystemComponent);
	// TODO Fix its level problem impacting MMC to work
	// ExperienceComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
	
}

// FIXME enemy should have identical attribute and abilities
void ABinggyEnemy::InitializeDefaultAttributes() const
{
	
	UUtilityLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
	UUtilityLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
}
