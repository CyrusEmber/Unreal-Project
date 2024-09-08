// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyGameplayAbility_Spell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Binggy/AbilitySystem/BinggyGameplayTags.h"
// #include "Binggy/Character/Component/CombatComponent.h"
#include "Binggy/UtilityLibrary.h"
#include "Binggy/Character/BinggyCharacter.h"
#include "Binggy/Character/Component/CombatComponent.h"
#include "Binggy/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"

#define LINE_TRACE_LENGTH 80000.f
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
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, 15.f);
	
	// Projectile->DamageEffectSpecHandle = SpecHandle;


	FireSpell(SpecHandle);
	
}

void UBinggyGameplayAbility_Spell::FireSpell(const FGameplayEffectSpecHandle& SpecHandle)
{
	if (bCanFire)
	{
		bCanFire = false;
		FHitResult HitResult;
		UUtilityLibrary::TraceUnderCrosshairByVisibility(HitResult, this);
		if (ABinggyCharacter* BinggyCharacter = GetBinggyCharacterFromActorInfo()) {
			BinggyCharacter->PlayFiringMontage(false);
			BinggyCharacter->GetCombatComponent()->EquippedWeapon->FireAbility(HitResult.ImpactPoint, SpecHandle);
		}
		StartFireTimer();
	}


}

void UBinggyGameplayAbility_Spell::StartFireTimer()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Started"));
	/*if (EquippedWeapon == nullptr || Character == nullptr) {
		return;
	}*/
	if (ABinggyCharacter* BinggyCharacter = GetBinggyCharacterFromActorInfo())
	{
		BinggyCharacter->GetWorldTimerManager().SetTimer(FireTimer, this, &UBinggyGameplayAbility_Spell::FireTimerFinished, FireDelay);
	}
	

}

void UBinggyGameplayAbility_Spell::FireTimerFinished()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Finished"));
	bCanFire = true;
	/*if (bFireButtonPressed) {
		Fire();
	}*/
}

