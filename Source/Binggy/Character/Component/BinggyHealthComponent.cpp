// Fill out your copyright notice in the Description page of Project Settings.


#include "Binggy/Character/Component/BinggyHealthComponent.h"

#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/AbilitySystem/Attributes/BinggyAttributeSet.h"

// Sets default values for this component's properties
UBinggyHealthComponent::UBinggyHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	
	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	AbilitySet = nullptr;
}

void UBinggyHealthComponent::InitializeWithAbilitySystem(UBinggyAbilitySystemComponent* InASC)
{
	AbilitySystemComponent = InASC;
	AbilitySet = AbilitySystemComponent->GetSet<UBinggyAttributeSet>();
	AbilitySet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	AbilitySet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChanged);

	// Initialize default values, TODO: driven by a spread sheet and SetNumericAttributeBase in Lyra
	OnHealthChanged.Broadcast(AbilitySet->GetHealth());
	OnMaxHealthChanged.Broadcast(AbilitySet->GetMaxHealth());
}

void UBinggyHealthComponent::UninitializeFromAbilitySystem()
{
	if (AbilitySet)
	{
		AbilitySet->OnHealthChanged.RemoveAll(this);
		AbilitySet->OnMaxHealthChanged.RemoveAll(this);
		// AbilitySet->OnOutOfHealth.RemoveAll(this);
	}

	AbilitySet = nullptr;
	AbilitySystemComponent = nullptr;
}

float UBinggyHealthComponent::GetHealth() const
{
	return AbilitySet ? AbilitySet->GetHealth() : 0.0f;
}

float UBinggyHealthComponent::GetMaxHealth() const
{
	return AbilitySet ? AbilitySet->GetMaxHealth() : 0.0f;
}

float UBinggyHealthComponent::GetHealthNormalized() const
{
	if (AbilitySet)
	{
		const float Health = AbilitySet->GetHealth();
		const float MaxHealth = AbilitySet->GetMaxHealth();

		return ((MaxHealth > 0.0f) ? (Health / MaxHealth) : 0.0f);
	}

	return 0.0f;
}


// Called when the game starts
void UBinggyHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UBinggyHealthComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();
	Super::OnUnregister();
}

void UBinggyHealthComponent::HandleHealthChanged(float NewValue)
{
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Event Fired!")); 
	OnHealthChanged.Broadcast(NewValue);
}

void UBinggyHealthComponent::HandleMaxHealthChanged(float NewValue)
{
	OnMaxHealthChanged.Broadcast(NewValue);
}


// Called every frame
void UBinggyHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

