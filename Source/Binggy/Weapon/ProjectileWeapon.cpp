// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"

#include "Casing.h"
#include "Projectile.h"
#include "Engine/SkeletalMeshSocket.h"

void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	if (!HasAuthority()) {
		return;
	}
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
	if (MuzzleSocket) {
		FTransform SocketTransform = MuzzleSocket->GetSocketTransform(GetWeaponMesh());
		// From muzzle to hit location from trace result
		FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator ToTargetRotation = ToTarget.Rotation();
		if (ProjectileClass && InstigatorPawn) {
			UWorld* World = GetWorld(); 
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = InstigatorPawn;
			if (World) {
				AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(), ToTargetRotation, SpawnParams);
			}
		}
	}

	
}

// TODO: change the project class
void AProjectileWeapon::FireAbility(const FVector& HitTarget, const FDamageEffectParams& ProjectileDamageEffectParams, TSubclassOf<AProjectile> InProjectileClass)
{
	/*Super::Fire(HitTarget);*/
	if (!HasAuthority()) {
		return;
	}
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
	if (MuzzleSocket) {
		FTransform SocketTransform = MuzzleSocket->GetSocketTransform(GetWeaponMesh());
		// From muzzle to hit location from trace result
		FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator ToTargetRotation = ToTarget.Rotation();
		if (InProjectileClass && InstigatorPawn) {
			UWorld* World = GetWorld(); 
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = InstigatorPawn;
			if (World) {
				// TODO: Defered Spawn?
				AProjectile* Projectile = World->SpawnActor<AProjectile>(InProjectileClass, SocketTransform.GetLocation(), ToTargetRotation, SpawnParams);
				
				Projectile->DamageEffectParams = ProjectileDamageEffectParams;
			}
		}
	}

	if (FireAnimation) {
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}
	if (CasingClass) {
		const USkeletalMeshSocket* AmmoEjectSocket = GetWeaponMesh()->GetSocketByName(FName("AmmoEject"));
		if (AmmoEjectSocket) {
			FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(GetWeaponMesh());

			UWorld* World = GetWorld();
			if (World) {
				World->SpawnActor<ACasing>(CasingClass, SocketTransform.GetLocation(), SocketTransform.GetRotation().Rotator());
			}
		}
	}
}
