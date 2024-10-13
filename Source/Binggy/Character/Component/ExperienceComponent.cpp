// Fill out your copyright notice in the Description page of Project Settings.


#include "ExperienceComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/AbilitySystem/BinggyGameplayTags.h"
#include "Binggy/AbilitySystem/Attributes/BinggyAttributeSet.h"
#include "Binggy/AbilitySystem/Attributes/BinggyExperienceSet.h"
#include "Binggy/AbilitySystem/Data/LevelInfo.h"
#include "Binggy/Character/BinggyCharacter.h"

class UBinggyAttributeSet;
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

void UExperienceComponent::UpdateAttribute(const FGameplayTag& AttributeTag)
{
	ServerUpgradeAttribute(AttributeTag);
}

void UExperienceComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	// Get server ASC
	UAbilitySystemComponent* ServerASC = Cast<ABinggyCharacter>(GetOwner())->GetAbilitySystemComponent();
	FGameplayEffectContextHandle EffectContext = ServerASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	// TODO Specify the level
	FGameplayEffectSpecHandle SpecHandle = ServerASC->MakeOutgoingSpec(AttributeEffectClass, 1, EffectContext);
	FBinggyGameplayTags GameplayTags = FBinggyGameplayTags::Get();
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Experience_AttributePoints, -1.f);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AttributeTag, 1.f);
	
	// Apply the effect to the character
	ServerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

// Called when the game starts
void UExperienceComponent::BeginPlay()
{
	Super::BeginPlay();
	// check
	check(AttributeEffectClass);
}

void UExperienceComponent::HandleExperienceChanged(const FOnAttributeChangeData& Data)
{
	// Level up check
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
	if (Data.OldValue == Data.NewValue)
	{
		return;
	}
	const float AddLevel = Data.NewValue - Data.OldValue;
	if (GetOwnerRole() == ROLE_Authority)
	{
		UAbilitySystemComponent* ServerASC = Cast<ABinggyCharacter>(GetOwner())->GetAbilitySystemComponent();
		FGameplayEffectContextHandle EffectContext = ServerASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		// TODO Specify the level
		FGameplayEffectSpecHandle SpecHandle = ServerASC->MakeOutgoingSpec(AttributeEffectClass, 1, EffectContext);
		FBinggyGameplayTags GameplayTags = FBinggyGameplayTags::Get();
		
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Experience_AttributePoints, AddLevel);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Experience_SkillPoints, AddLevel);
		// Handle health and mana refill
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Vital_Health,  AbilitySystemComponent->GetSet<UBinggyAttributeSet>()->GetMaxHealth());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Vital_Mana,  AbilitySystemComponent->GetSet<UBinggyAttributeSet>()->GetMaxMana());

		// UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AttributeTag, 1.f);
	
		// Apply the effect to the character
		ServerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
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
	// Only send the information on the client is able to do the trick
	// APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (GetOwnerRole() == ROLE_Authority)
	{
		UAbilitySystemComponent* ServerASC = Cast<ABinggyCharacter>(GetOwner())->GetAbilitySystemComponent();
		FGameplayEffectContextHandle EffectContext = ServerASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = ServerASC->MakeOutgoingSpec(AttributeEffectClass, 1, EffectContext);
		FBinggyGameplayTags GameplayTags = FBinggyGameplayTags::Get();
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Experience_Level, AddLevel);
	
		// Apply the effect to the character
		ServerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		// Lagecy 
		/*const FBinggyGameplayTags& GameplayTags = FBinggyGameplayTags::Get();
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
		Payload.EventMagnitude = AbilitySystemComponent->GetSet<UBinggyAttributeSet>()->GetMaxHealth();
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AbilitySystemComponent->GetAvatarActor(), Payload.EventTag, Payload);
		Payload.EventTag = GameplayTags.Attributes_Vital_Mana;
		Payload.EventMagnitude = AbilitySystemComponent->GetSet<UBinggyAttributeSet>()->GetMaxMana();
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AbilitySystemComponent->GetAvatarActor(), Payload.EventTag, Payload);*/
	}
	
}

/*void UExperienceComponent::ServerOnLevelUp_Implementation(const float AddLevel) const
{

}*/

void UExperienceComponent::InitializeWithAbilitySystem(UBinggyAbilitySystemComponent* InASC)
{
	AbilitySystemComponent = InASC;
	ExperienceSet = AbilitySystemComponent->GetSet<UBinggyExperienceSet>();
	// TODO: Initialized 4 times?

	// Set experience variable
	check(LevelInfo);
	SetLevelExperience(GetLevel());
		
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
	AbilitySystemComponent = nullptr;
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ExperienceSet->GetExperienceAttribute()).RemoveAll(this);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ExperienceSet->GetLevelAttribute()).RemoveAll(this);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ExperienceSet->GetAttributePointsAttribute()).RemoveAll(this);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ExperienceSet->GetSkillPointsAttribute()).RemoveAll(this);
}

