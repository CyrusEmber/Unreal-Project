// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyGameplayAbility_Interact.h"

#include "AbilitySystemComponent.h"
#include "Interaction/InteractionBlueprintLibrary.h"
#include "Interaction/InteractionOption.h"
#include "Interaction/Tasks/AbilityTask_NearbyInteraction.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"

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

	// TODO it suppose to show something on the actor
	for (const FInteractionOption& InteractionOption : InteractiveOptions)
	{
		AActor* InteractableTargetActor = UInteractionBlueprintLibrary::GetActorFromInteractableTarget(InteractionOption.InteractableTarget);
		/*DefaultInteractionWidgetClass.LoadSynchronous();*/
		// TSoftClassPtr<UUserWidget> InteractionWidgetClass = InteractionOption.InteractionWidgetClass.IsNull() ? DefaultInteractionWidgetClass : InteractionOption.InteractionWidgetClass;
		
		
		if (DefaultInteractionWidgetClass)
		{
			DebugWidgetComponent = InteractableTargetActor->FindComponentByClass<UWidgetComponent>();
			if (DebugWidgetComponent)
			{
				// Update the widget class of the Widget Component
				DebugWidgetComponent->SetWidgetClass(DefaultInteractionWidgetClass);

				// Optional: Call InitWidget to refresh the Widget Component
				DebugWidgetComponent->InitWidget();
				
				DebugWidgetComponent->SetVisibility(true);
				DebugWidgetComponent->SetHiddenInGame(false);
				DebugWidgetComponent->SetDrawSize(FVector2D(200, 200));
			}
		}
	}

	// TODO Debug
	if (InteractiveOptions.Num() == 0)
	{
		DebugWidgetComponent->SetVisibility(false);
	}

	CurrentOptions = InteractiveOptions;
	SetCurrentInteractionOptionsInASC(CurrentOptions);
}





