// Fill out your copyright notice in the Description page of Project Settings.


#include "Casing.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ACasing::ACasing()
{
	PrimaryActorTick.bCanEverTick = false;
	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>("CasingMesh");
	SetRootComponent(CasingMesh);


	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	//CasingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//CasingMesh->SetCollisionObjectType(ECC_PhysicsBody);
	CasingMesh->SetNotifyRigidBodyCollision(true);
	ShellEjectionImpluse = 4.f;
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();

	CasingMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpluse);
	CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);
	SoundPlayed = false;
}

void ACasing::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpluse, const FHitResult& HitResult)
{
	if (ShellSound && !SoundPlayed) {
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
		SoundPlayed = true;
	}
	SetLifeSpan(10.0f);
}


