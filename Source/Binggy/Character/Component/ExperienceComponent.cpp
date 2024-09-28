// Fill out your copyright notice in the Description page of Project Settings.


#include "ExperienceComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/AbilitySystem/BinggyGameplayTags.h"
#include "Binggy/AbilitySystem/Attributes/BinggyExperienceSet.h"
#include "Binggy/AbilitySystem/Data/LevelInfo.h"

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
	
}

void UExperienceComponent::HandleExperienceChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue >= CurrentLevelExperience)
	{
		const int32 NewLevel = LevelInfo->GetLevelByXP(FMath::RoundToInt(Data.NewValue));
		SetLevelExperience(NewLevel);
		OnLevelUp(NewLevel - GetLevel());
	}
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

void UExperienceComponent::SetLevelExperience(float InLevel)
{
	const int32 IntLevel = FMath::RoundToInt(InLevel);
	PreviousLevelExperience = LevelInfo->GetXPByLevel(IntLevel - 1);
	CurrentLevelExperience = LevelInfo->GetXPByLevel(IntLevel);
}

// TODO: potential batch operation
void UExperienceComponent::OnLevelUp(const float AddLevel) const
{
	// Only send the information on the client is able to do the trick, there is some experience BUG: TODO:
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn && OwnerPawn->IsLocallyControlled())
	{
		const FBinggyGameplayTags& GameplayTags = FBinggyGameplayTags::Get();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Level Up Send to actor name: %s"), *AbilitySystemComponent->GetAvatarActor()->GetName()));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Level Up Owner name: %s"), *AbilitySystemComponent->GetOwner()->GetName()));
		// Add Attribute points
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Experience_AttributePoints;
		Payload.EventMagnitude = AddLevel;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AbilitySystemComponent->GetAvatarActor(), Payload.EventTag, Payload);

		// Add level
		Payload.EventTag = GameplayTags.Attributes_Experience_Level;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AbilitySystemComponent->GetAvatarActor(), Payload.EventTag, Payload);

		// Add skill points
		Payload.EventTag = GameplayTags.Attributes_Experience_SkillPoints;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AbilitySystemComponent->GetAvatarActor(), Payload.EventTag, Payload);

		// Refill health and mana
		Payload.EventTag = GameplayTags.Attributes_Vital_Health;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AbilitySystemComponent->GetAvatarActor(), Payload.EventTag, Payload);
		Payload.EventTag = GameplayTags.Attributes_Vital_Mana;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AbilitySystemComponent->GetAvatarActor(), Payload.EventTag, Payload);
	}
	
}

void UExperienceComponent::InitializeWithAbilitySystem(UBinggyAbilitySystemComponent* InASC)
{
	AbilitySystemComponent = InASC;
	ExperienceSet = AbilitySystemComponent->GetSet<UBinggyExperienceSet>();
	// TODO: Initialized 4 times?

	// Set experience variable
	check(LevelInfo);
	SetLevelExperience(GetLevel());

		// Debug
		UGameplayEffect* GEDebug = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Bounty")));
		GEDebug->DurationPolicy = EGameplayEffectDurationType::Instant;

		int32 Idx = GEDebug->Modifiers.Num();
		GEDebug->Modifiers.SetNum(Idx + 1);

		FGameplayModifierInfo& AttributePoints = GEDebug->Modifiers[Idx];
		AttributePoints.ModifierMagnitude = FScalableFloat(1.0f);
		AttributePoints.ModifierOp = EGameplayModOp::Additive;
		AttributePoints.Attribute = UBinggyExperienceSet::GetAttributePointsAttribute();

		AbilitySystemComponent->ApplyGameplayEffectToSelf(GEDebug, 1.0f, AbilitySystemComponent->MakeEffectContext());
		
	// Initialize default values, TODO: driven by a spread sheet and SetNumericAttributeBase in Lyra
	OnExperienceChanged.Broadcast(ExperienceSet->GetExperience());
	OnLevelChanged.Broadcast(ExperienceSet->GetLevel());
	OnAttributePointsChanged.Broadcast(ExperienceSet->GetAttributePoints());
	OnSkillPointsChanged.Broadcast(ExperienceSet->GetSkillPoints());

	// Bind Callbacks
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ExperienceSet->GetExperienceAttribute()).AddUObject(this, &UExperienceComponent::HandleExperienceChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ExperienceSet->GetLevelAttribute()).AddUObject(this, &UExperienceComponent::HandleLevelChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ExperienceSet->GetAttributePointsAttribute()).AddUObject(this, &UExperienceComponent::HandleAttributePointsChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ExperienceSet->GetSkillPointsAttribute()).AddUObject(this, &UExperienceComponent::HandleSkillPointsChanged);
}

void UExperienceComponent::UninitializeFromAbilitySystem()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ExperienceSet->GetExperienceAttribute()).RemoveAll(this);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ExperienceSet->GetLevelAttribute()).RemoveAll(this);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ExperienceSet->GetLevelAttribute()).RemoveAll(this);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ExperienceSet->GetLevelAttribute()).RemoveAll(this);
}

