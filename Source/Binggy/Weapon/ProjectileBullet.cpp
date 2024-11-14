// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBullet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "UtilityLibrary.h"
#include "GameFramework/Character.h"

void AProjectileBullet::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpluse, const FHitResult& HitResult)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter) {
		AController* OwnerController = OwnerCharacter->Controller;
		if (OwnerController) {
			// UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
		}
	}
	if (HasAuthority())
	{
		// check(DamageEffectSpecHandle.Data);
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// TODO Hit result
			DamageEffectParams.HitResult = HitResult;
			const FVector ImpulseDirection = GetActorForwardVector() * DamageEffectParams.ImpulseMagnitude;
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			DamageEffectParams.ImpulseDirection = ImpulseDirection;
			UUtilityLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		
	}
	Destroy();
}
