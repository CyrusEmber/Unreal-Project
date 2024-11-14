// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSpawner.h"

#include "Inventory/BinggyInventoryComponent.h"
#include "Inventory/BinggyInventoryItemDefinition.h"
#include "Inventory/InventoryFragment_InventoryItem.h"
#include "Inventory/InventoryFragment_PickupItem.h"
#include "Components/CapsuleComponent.h"

class UInventoryFragment_PickupItem;
// Sets default values
AWeaponSpawner::AWeaponSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CollisionVolume = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionVolume"));
	CollisionVolume->InitCapsuleSize(80.f, 80.f);
	

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AWeaponSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AWeaponSpawner::OnOverlapBegin);
	}
	
}

// Called every frame
void AWeaponSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponSpawner::OnConstruction(const FTransform& Transform)
{
	if (WeaponDefinition != nullptr)
	{
		//const UInventoryFragment_InventoryItem* PickupItemFragment =  Cast<UInventoryFragment_InventoryItem>(WeaponDefinition->FindFragmentByClass(UInventoryFragment_InventoryItem::StaticClass()));
		//WeaponMesh->SetStaticMesh(PickupItemFragment->DebugDisplayMesh);
	}	
}

void AWeaponSpawner::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (APawn* Pawn = Cast<APawn>(OtherActor))
	{
		// TSubclassOf<UBinggyInventoryItemDefinition> WeaponItemDefinition = WeaponDefinition->GetClass();
		// Only call the function on server
		if (HasAuthority()) 
		{
			DebugInventory(WeaponDefinition, Pawn);
		}
		
	}
}

bool AWeaponSpawner::DebugInventory(TSubclassOf<UBinggyInventoryItemDefinition> WeaponItemClass, APawn* ReceivingPawn)
{
	UBinggyInventoryComponent* InventoryComponent = ReceivingPawn->FindComponentByClass<UBinggyInventoryComponent>();
	
	if (InventoryComponent)
	{
		// TODO debug
		InventoryComponent->AddItemDefinition(WeaponItemClass);
		return true;
	}

	return false;
}

