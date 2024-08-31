// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyGameplayAbility_Spell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BinggyGameplayTags.h"
#include "Binggy/BinggyComponent/CombatComponent.h"
#include "Binggy/Character/BinggyCharacter.h"

void UBinggyGameplayAbility_Spell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;

	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

	FBinggyGameplayTags GameplayTags = FBinggyGameplayTags::Get();
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, 50.f);
	
	// Projectile->DamageEffectSpecHandle = SpecHandle;
	
	ABinggyCharacter* BinggyCharacter = Cast<ABinggyCharacter>(GetAvatarActorFromActorInfo());

	if (BinggyCharacter)
	{
		
		BinggyCharacter->GetCombatComponent()->Fire();
	}
}
