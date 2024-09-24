// Fill out your copyright notice in the Description page of Project Settings.


#include "ExperienceComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/AbilitySystem/Attributes/BinggyExperienceSet.h"

// Sets default values for this component's properties
UExperienceComponent::UExperienceComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	
	AbilitySystemComponent = nullptr;
	ExperienceSet = nullptr;
}


float UExperienceComponent::GetExperience() const
{
	return ExperienceSet->GetExperience();
}

float UExperienceComponent::GetLevel() const
{
	return ExperienceSet->GetLevel();
}

float UExperienceComponent::GetExperienceNormalized() const
{
	return (GetExperience() - PreviousLevelExperience) / CurrentLevelExperience;
}

float UExperienceComponent::GetSkillPoints() const
{
	return ExperienceSet->GetSkillPoints();
}

float UExperienceComponent::GetAttributePoints() const
{
	return ExperienceSet->GetAttributePoints();
}

void UExperienceComponent::ClearPointsDelegateBinding()
{
	OnAttributePointsChanged.Clear();
	OnSkillPointsChanged.Clear();
}

// Called when the game starts
void UExperienceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UExperienceComponent::HandleExperienceChanged(const FOnAttributeChangeData& Data)
{
	OnExperienceChanged.Broadcast(Data.NewValue);
}

void UExperienceComponent::HandleLevelChanged(const FOnAttributeChangeData& Data)
{
	OnLevelChanged.Broadcast(Data.NewValue);
}

void UExperienceComponent::HandleAttributePointsChanged(const FOnAttributeChangeData& Data)
{
	OnAttributePointsChanged.Broadcast(Data.NewValue);
}

void UExperienceComponent::HandleSkillPointsChanged(const FOnAttributeChangeData& Data)
{
	OnSkillPointsChanged.Broadcast(Data.NewValue);
}

void UExperienceComponent::InitializeWithAbilitySystem(UBinggyAbilitySystemComponent* InASC)
{
	AbilitySystemComponent = InASC;
	ExperienceSet = AbilitySystemComponent->GetSet<UBinggyExperienceSet>();

	// Initialize default values, TODO: driven by a spread sheet and SetNumericAttributeBase in Lyra
	OnExperienceChanged.Broadcast(ExperienceSet->GetExperience());
	OnLevelChanged.Broadcast(ExperienceSet->GetLevel());
	OnAttributePointsChanged.Broadcast(ExperienceSet->GetAttributePoints());
	OnSkillPointsChanged.Broadcast(ExperienceSet->GetSkillPoints());

	// Bind Callbacks
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ExperienceSet->GetExperienceAttribute()).AddUObject(this, &UExperienceComponent::HandleExperienceChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ExperienceSet->GetLevelAttribute()).AddUObject(this, &UExperienceComponent::HandleLevelChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ExperienceSet->GetAttributePointsAttribute()).AddUObject(this, &UExperienceComponent::HandleAttributePointsChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ExperienceSet->GetSkillPointsAttribute()).AddUObject(this, &UExperienceComponent::HandleLevelChanged);
}

void UExperienceComponent::UninitializeFromAbilitySystem()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ExperienceSet->GetExperienceAttribute()).RemoveAll(this);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ExperienceSet->GetLevelAttribute()).RemoveAll(this);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ExperienceSet->GetLevelAttribute()).RemoveAll(this);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ExperienceSet->GetLevelAttribute()).RemoveAll(this);
}

