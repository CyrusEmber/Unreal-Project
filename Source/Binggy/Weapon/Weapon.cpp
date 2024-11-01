// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Binggy/Character/BinggyCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"
#include "Casing.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Binggy/Binggy.h"
#include "Binggy/AbilitySystem/Abilities/BinggyAbilityTypes.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	//WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
	PickupWidget->SetDrawAtDesiredSize(true);
	PickupWidget->SetWidgetSpace(EWidgetSpace::Screen);

	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	this->SetReplicateMovement(true);
	
	if (HasAuthority()) {
		// TODO: Set it in Cpp?
		// AreaSphere->SetCollisionObjectType(ECC_InvisibleArea);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::EndSphereOverlap);
	}

	if (PickupWidget) {
		PickupWidget->SetVisibility(false);
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponState);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	ABinggyCharacter* BinggyCharacter = Cast<ABinggyCharacter>(OtherActor);
	if (BinggyCharacter) {
		BinggyCharacter->SetOverlappingWeapon(this);
	}
}

void AWeapon::EndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	ABinggyCharacter* BinggyCharacter = Cast<ABinggyCharacter>(OtherActor);
	if (BinggyCharacter) {
		BinggyCharacter->SetOverlappingWeapon(nullptr);
	}
}

void AWeapon::SetMeshByWeaponState()
{
	switch(WeaponState) {
	case(EWeaponState::EWS_Equipped):
		ShowPickupWidget(false);
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case(EWeaponState::EWS_Dropped):
		ShowPickupWidget(true);
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// I don't want player to kick weapon away.
		WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		WeaponMesh->SetCollisionResponseToChannel(ECC_HitBox, ECR_Ignore);
		WeaponMesh->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECR_Ignore);
		break;
	}
}

// Only excecute on clients
void AWeapon::OnRep_WeaponState()
{
	//UOverHeadWidget* DebugWidget = Cast<UOverHeadWidget>(PickupWidget);
	//FText DebugText = DebugWidget->GetDisplayText();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, DebugText.ToString());
	
}

// Only excecuted on server
void AWeapon::SetWeaponState(EWeaponState State)
{
	// Setting this variable will call the onrep function propagate to client
	WeaponState = State;
	SetMeshByWeaponState();
}

void AWeapon::ShowPickupWidget(bool bShowWidget)
{
	if (PickupWidget) {
		PickupWidget->SetVisibility(bShowWidget);

	}
}

// Firing on all machines
void AWeapon::Fire(const FVector& HitTarget)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Fire Debug")); 
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

void AWeapon::FireAbility(const FVector& HitTarget, const FDamageEffectParams& ProjectileDamageEffectParams, TSubclassOf<AProjectile>
                          InProjectileClass)
{
}

void AWeapon::Drop()
{
	SetWeaponState(EWeaponState::EWS_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	WeaponMesh->DetachFromComponent(DetachRules);

	SetOwner(nullptr);
}

