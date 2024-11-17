// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/AbilityTask_WaitInputTriggered.h"

#include "EnhancedInputComponent.h"


UAbilityTask_WaitInputTriggered* UAbilityTask_WaitInputTriggered::WaitInputTriggered(UGameplayAbility* OwningAbility,
	APlayerController* PC, UInputAction* InputAction, float Interval)
{
	UAbilityTask_WaitInputTriggered* MyObj = NewAbilityTask<UAbilityTask_WaitInputTriggered>(OwningAbility);
	MyObj->PlayerController = PC;
	MyObj->InputAction = InputAction;
	MyObj->Interval = Interval;
	return MyObj;
}

void UAbilityTask_WaitInputTriggered::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
	if (PlayerController.IsValid())
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->RemoveBinding(*BindingStart);
			EnhancedInputComponent->RemoveBinding(*BindingEnd);
			BindingStart = nullptr;
			BindingEnd = nullptr;
		}
	}
}

void UAbilityTask_WaitInputTriggered::Activate()
{
	Super::Activate();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
	{
		BindingStart = &EnhancedInputComponent->BindAction(InputAction.Get(), ETriggerEvent::Started,
				this, &UAbilityTask_WaitInputTriggered::InputStartCallback);

		BindingStart = &EnhancedInputComponent->BindAction(InputAction.Get(), ETriggerEvent::Triggered,
		this, &UAbilityTask_WaitInputTriggered::InputTriggeredCallback);

		BindingEnd = &EnhancedInputComponent->BindAction(InputAction.Get(), ETriggerEvent::Completed,
				this, &UAbilityTask_WaitInputTriggered::InputCompleteCallback);
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
	// Start broadcast
	InputTriggered.Broadcast(bRotateRight);
}

void UAbilityTask_WaitInputTriggered::InputTriggeredCallback(const FInputActionValue& Value)
{
	if (Value.Get<float>() == 1.f)
	{
		bRotateRight = true;
	} else
	{
		bRotateRight = false;
	}
}

void UAbilityTask_WaitInputTriggered::InputCompleteCallback(const FInputActionValue& Value)
{
	bRotateRight = NULL;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UAbilityTask_WaitInputTriggered::InputTrigger()
{
	InputTriggered.Broadcast(bRotateRight);
}
