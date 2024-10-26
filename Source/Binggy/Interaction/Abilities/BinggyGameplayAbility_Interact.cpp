// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyGameplayAbility_Interact.h"

#include "AbilitySystemComponent.h"
#include "Binggy/AbilitySystem/BinggyGameplayTags.h"
#include "Binggy/Interaction/InteractionBlueprintLibrary.h"
#include "Binggy/Interaction/InteractionOption.h"
#include "Binggy/Interaction/Tasks/AbilityTask_NearbyInteraction.h"

UBinggyGameplayAbility_Interact::UBinggyGameplayAbility_Interact()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UBinggyGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystem && AbilitySystem->GetOwnerRole() == ROLE_Authority)
	{
		UAbilityTask_NearbyInteraction* Task = UAbilityTask_NearbyInteraction::GrantAbilitiesForNearbyInteractors(this, InteractionScanRange, InteractionScanRate);
		Task->ReadyForActivation();
	}
}

void UBinggyGameplayAbility_Interact::UpdateInteractions(const TArray<FInteractionOption>& InteractiveOptions)
{
	// TODO it suppose to use ULyraIndicatorManagerComponent to show something on the actor

	CurrentOptions = InteractiveOptions;
	SetCurrentInteractionOptionsInASC(CurrentOptions);
}





