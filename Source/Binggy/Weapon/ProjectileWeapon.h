// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

struct FGameplayEffectSpecHandle;
/**
 * 
 */
class AProjectile;
UCLASS()
class BINGGY_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	virtual void Fire(const FVector& HitTarget) override;

	// TODO: Make it virtual
	virtual void FireAbility(const FVector& HitTarget, const FDamageEffectParams& ProjectileDamageEffectParams) override;

private:
	UPROPERTY(EditAnywhere, Category = "Projectile Weapon")
	TSubclassOf<AProjectile> ProjectileClass;
	
};
