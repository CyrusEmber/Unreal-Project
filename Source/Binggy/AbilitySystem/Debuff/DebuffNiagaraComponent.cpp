// Fill out your copyright notice in the Description page of Project Settings.


#include "DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/Character/Component/BinggyHealthComponent.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UDebuffNiagaraComponent::InitializeWithAbilitySystem(UBinggyAbilitySystemComponent* InASC)
{
	// ASC will be valid after InitializeWithAbilitySystem
	
	if (InASC)
	{
		AbilitySystemComponent = InASC;
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Hit Location: %s"), *DebuffTag.GetTagName().ToString()));
		InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffChanged);
	}

	UBinggyHealthComponent* HealthComponent = UBinggyHealthComponent::FindHealthComponent(GetOwner());
	if (HealthComponent)
	{
		HealthComponent->OnOutOfHealthChanged.AddDynamic(this, &UDebuffNiagaraComponent::HandleOnOutOfHealth);
	}
}

void UDebuffNiagaraComponent::UninitializeFromAbilitySystem()
{
	AbilitySystemComponent->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
}

void UDebuffNiagaraComponent::DebuffChanged(const FGameplayTag CallbackTag, int32 NewCount)
{

	if (NewCount > 0)
	{
		Activate();
	} else
	{
		Deactivate();
	}
}

void UDebuffNiagaraComponent::HandleOnOutOfHealth(float NewValue)
{
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Death of Niagaracomponent")); 

	Deactivate();
}
