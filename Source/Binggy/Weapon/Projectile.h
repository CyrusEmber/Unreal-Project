// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Binggy/AbilitySystem/Abilities/BinggyAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UParticleSystem;
class USoundCue;
class UParticleSystemComponent;
/*
 * Projectile class is of collision type projectile, and by default would block skeletal mesh
 * 
 */
UCLASS()
class BINGGY_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	// Play sound and impact animation
	virtual void Destroyed() override;
	// TODO: set the function and make spec handle protected
	void SetDamageEffectSpecHandleByDefault();
	
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn=true))
	FDamageEffectParams DamageEffectParams;
	

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpluse, const FHitResult& HitResult);

	UPROPERTY(EditAnywhere)
	float Damage = 0.f;




private:
	UPROPERTY(EditAnywhere, Category = "Projectile")
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UParticleSystem* Tracer;

	UParticleSystemComponent* TracerComponent;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UParticleSystem* ImpactParticle;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	USoundCue* ImpactSound;

public:	
	

};
