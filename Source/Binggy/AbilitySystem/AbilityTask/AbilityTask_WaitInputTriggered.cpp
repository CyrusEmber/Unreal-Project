// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/AbilityTask_WaitInputTriggered.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"


UAbilityTask_WaitInputTriggered* UAbilityTask_WaitInputTriggered::WaitInputTriggered(UGameplayAbility* OwningAbility,
	UInputAction* InputAction, float Interval)
{
	UAbilityTask_WaitInputTriggered* MyObj = NewAbilityTask<UAbilityTask_WaitInputTriggered>(OwningAbility);
	MyObj->InputAction = InputAction;
	MyObj->Interval = Interval;
	return MyObj;
}

void UAbilityTask_WaitInputTriggered::OnDestroy(bool AbilityEnded)
{
	if (Ability)
	{
		APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
		if (PC)
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent))
			{
				EnhancedInputComponent->RemoveBinding(*BindingStart);
				EnhancedInputComponent->RemoveBinding(*BindingTrigger);
				EnhancedInputComponent->RemoveBinding(*BindingEnd);
				BindingStart = nullptr;
				BindingTrigger = nullptr;
				BindingEnd = nullptr;
			}
		}
	}
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_WaitInputTriggered::Activate()
{
	Super::Activate();
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	DelegateHandle = ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputPressed, GetAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(this, &UAbilityTask_WaitInputTriggered::OnPressCallback);

	if (IsForRemoteClient())
	{
		if (!ASC->CallReplicatedEventDelegateIfSet(EAbilityGenericReplicatedEvent::InputPressed, GetAbilitySpecHandle(), GetActivationPredictionKey()))
		{
			SetWaitingOnRemotePlayerData();
		}
	}
	

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (PC)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			BindingStart = &EnhancedInputComponent->BindAction(InputAction.Get(), ETriggerEvent::Started,
					this, &UAbilityTask_WaitInputTriggered::InputStartCallback);

			BindingTrigger = &EnhancedInputComponent->BindAction(InputAction.Get(), ETriggerEvent::Triggered,
			this, &UAbilityTask_WaitInputTriggered::InputTriggeredCallback);

			BindingEnd = &EnhancedInputComponent->BindAction(InputAction.Get(), ETriggerEvent::Completed,
					this, &UAbilityTask_WaitInputTriggered::InputCompleteCallback);
		}
	}
	

}

void UAbilityTask_WaitInputTriggered::InputStartCallback(const FInputActionValue& Value)
{
	if (Value.Get<float>() == 1.f)
	{
		bRotateRight = true;
	} else
	{
		bRotateRight = false;
	}
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAbilityTask_WaitInputTriggered::InputTrigger, Interval, true);

	// Initial broadcast before the timer
	// InputTriggered.Broadcast(bRotateRight);
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	ASC->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, GetAbilitySpecHandle(), GetActivationPredictionKey(), ASC->ScopedPredictionKey);
}

void UAbilityTask_WaitInputTriggered::InputTriggeredCallback(const FInputActionValue& Value)
{
	bRotateRight = (Value.Get<float>() == 1.f);
}

void UAbilityTask_WaitInputTriggered::InputCompleteCallback(const FInputActionValue& Value)
{
	bRotateRight = NULL;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UAbilityTask_WaitInputTriggered::InputTrigger()
{
	// InputTriggered.Broadcast(bRotateRight); 
}

void UAbilityTask_WaitInputTriggered::OnPressCallback()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (!Ability || !ASC)
	{
		return;
	}
	ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputPressed, GetAbilitySpecHandle(), GetActivationPredictionKey()).Remove(DelegateHandle);

	FScopedPredictionWindow ScopedPrediction(ASC, IsPredictingClient());
	if (IsPredictingClient())
	{
		// Tell the server about this
		ASC->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, GetAbilitySpecHandle(), GetActivationPredictionKey(), ASC->ScopedPredictionKey);
	}
	else
	{
		ASC->ConsumeGenericReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, GetAbilitySpecHandle(), GetActivationPredictionKey());
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		InputTriggered.Broadcast(bRotateRight);
	}
}
