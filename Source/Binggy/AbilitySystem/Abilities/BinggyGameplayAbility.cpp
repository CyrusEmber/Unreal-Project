// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyGameplayAbility.h"

#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/Character/BinggyCharacter.h"

ABinggyCharacter* UBinggyGameplayAbility::GetBinggyCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ABinggyCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

UBinggyAbilitySystemComponent* UBinggyGameplayAbility::GetBinggyAbilitySystemFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<UBinggyAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}
