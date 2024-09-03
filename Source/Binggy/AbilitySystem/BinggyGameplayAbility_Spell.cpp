// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyGameplayAbility_Spell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BinggyGameplayTags.h"
// #include "Binggy/Character/Component/CombatComponent.h"
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
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, 50.f);
	
	// Projectile->DamageEffectSpecHandle = SpecHandle;


	FireSpell();
	
}

void UBinggyGameplayAbility_Spell::FireSpell()
{
	if (bCanFire)
	{
		bCanFire = false;
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		if (ABinggyCharacter* BinggyCharacter = GetBinggyCharacterFromActorInfo()) {
			BinggyCharacter->PlayFiringMontage(false);
			BinggyCharacter->GetCombatComponent()->EquippedWeapon->Fire(HitResult.ImpactPoint);
		}
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

void UBinggyGameplayAbility_Spell::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);
	if (bScreenToWorld) {
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + CrosshairWorldDirection * LINE_TRACE_LENGTH;

		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);

		if (!TraceHitResult.bBlockingHit) {
			TraceHitResult.ImpactPoint = End;
		}
		//else {
		//	DrawDebugSphere(GetWorld(), TraceHitResult.ImpactPoint, 12.f, 12, FColor::Red);
		//}
	}
}
