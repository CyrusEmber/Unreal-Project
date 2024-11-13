// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_TriggerInteract.h"

#include "AbilitySystem/BinggyGameplayTags.h"
#include "Interaction/InteractionBlueprintLibrary.h"
#include "Interaction/InteractionOption.h"

void UAbility_TriggerInteract::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAbility_TriggerInteract::TriggerInteraction(TArray<FInteractionOption> CurrentOptions)
{
	if (CurrentOptions.Num() == 0)
	{
		return;
	}
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystem)
	{
		// TODO: handle other options if there is any
		const FInteractionOption& InteractionOption = CurrentOptions[0];

		AActor* Instigator = GetAvatarActorFromActorInfo();
		AActor* InteractableTargetActor = UInteractionBlueprintLibrary::GetActorFromInteractableTarget(InteractionOption.InteractableTarget);
		
		const FGameplayTag& InteractionActiveTag = FBinggyGameplayTags::Get().Ability_Interaction_Active;

		// Allow the target to customize the event data we're about to pass in, in case the ability needs custom data
		// that only the actor knows. TODO add more
		FGameplayEventData Payload;
		Payload.EventTag = InteractionActiveTag;
		Payload.Instigator = Instigator;
		Payload.Target = InteractableTargetActor;

		// If needed we allow the interactable target to manipulate the event data so that for example, a button on the wall
		// may want to specify a door actor to execute the ability on, so it might choose to override Target to be the
		// door actor.
		//InteractionOption.InteractableTarget->CustomizeInteractionEventData(TAG_Ability_Interaction_Activate, Payload);

		// Grab the target actor off the payload we're going to use it as the 'avatar' for the interaction, and the
		// source InteractableTarget actor as the owner actor.
		AActor* TargetActor = const_cast<AActor*>(ToRawPtr(Payload.Target));

		// The actor info needed for the interaction.
		FGameplayAbilityActorInfo ActorInfo;
		ActorInfo.InitFromActor(InteractableTargetActor, TargetActor, InteractionOption.TargetAbilitySystem);

		// Trigger the ability using event tag.
		const bool bSuccess = InteractionOption.TargetAbilitySystem->TriggerAbilityFromGameplayEvent(
			InteractionOption.TargetInteractionAbilityHandle,
			&ActorInfo,
			InteractionActiveTag,
			&Payload,
			*InteractionOption.TargetAbilitySystem
		);
	}
}
