// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileBullet.generated.h"

/**
 * 
 */
UCLASS()
class BINGGY_API AProjectileBullet : public AProjectile
{
	GENERATED_BODY()

protected:
	virtual void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpluse, const FHitResult& HitResult) override;

	
};
