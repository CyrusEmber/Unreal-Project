// Fill out your copyright notice in the Description page of Project Settings.


#include "Binggy/Character/Component/BinggyHealthComponent.h"

#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/AbilitySystem/Attributes/BinggyAttributeSet.h"
#include "Binggy/AbilitySystem/BinggyGameplayTags.h"

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

	/*HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetDrawAtDesiredSize(true);
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);*/
}

void UBinggyHealthComponent::InitializeWithAbilitySystem(UBinggyAbilitySystemComponent* InASC)
{
	AbilitySystemComponent = InASC;
	AbilitySet = AbilitySystemComponent->GetSet<UBinggyAttributeSet>();
	AbilitySet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	AbilitySet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChanged);
	AbilitySet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);

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
		AbilitySet->OnOutOfHealth.RemoveAll(this);
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
	// Set Health bar attachment so that it is visible
	// HealthBar->SetupAttachment(GetOwner()->GetRootComponent());

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
	FGameplayTagContainer TagContainer;
	// Hit React
	TagContainer.AddTag(FBinggyGameplayTags::Get().GameplayEvent_HitReact);
	AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
	OnHealthChanged.Broadcast(NewValue);
}

void UBinggyHealthComponent::HandleMaxHealthChanged(float NewValue)
{
	OnMaxHealthChanged.Broadcast(NewValue);
}

void UBinggyHealthComponent::HandleOutOfHealth(float NewValue)
{
#if WITH_SERVER_CODE
	// Send the "GameplayEvent.Death" gameplay event through the owner's ability system.  This can be used to trigger a death gameplay ability.
	{
		FGameplayEventData Payload;
		Payload.EventTag = FBinggyGameplayTags::Get().GameplayEvent_Death;
		/*Payload.Instigator = DamageInstigator;*/
		Payload.Target = AbilitySystemComponent->GetAvatarActor();
		/*Payload.OptionalObject = DamageEffectSpec->Def;
		Payload.ContextHandle = DamageEffectSpec->GetEffectContext();
		Payload.InstigatorTags = *DamageEffectSpec->CapturedSourceTags.GetAggregatedTags();
		Payload.TargetTags = *DamageEffectSpec->CapturedTargetTags.GetAggregatedTags();
		Payload.EventMagnitude = DamageMagnitude;*/

		FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
		/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Death Tag Sent"));
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Activated Ability: %i"), AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload)));
		AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);*/
	}
#endif // #if WITH_SERVER_CODE
	
	// This directly activate the current ability owned by the ASC
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(FBinggyGameplayTags::Get().GameplayEvent_Death);
	AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);


}

/*UWidgetComponent* UBinggyHealthComponent::GetHealthBar()
{
	check(HealthBar);
	HealthBar->SetupAttachment(GetOwner()->GetRootComponent());
	return HealthBar;
}*/

// Called every frame
void UBinggyHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

