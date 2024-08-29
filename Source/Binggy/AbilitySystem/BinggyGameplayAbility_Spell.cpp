// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyGameplayAbility_Spell.h"

#include "Binggy/BinggyComponent/CombatComponent.h"
#include "Binggy/Character/BinggyCharacter.h"

void UBinggyGameplayAbility_Spell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;
	
	ABinggyCharacter* BinggyCharacter = Cast<ABinggyCharacter>(GetAvatarActorFromActorInfo());

	if (BinggyCharacter)
	{
		BinggyCharacter->GetCombatComponent()->Fire();
	}
}
