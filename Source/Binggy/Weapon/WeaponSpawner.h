// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponSpawner.generated.h"

class UCapsuleComponent;
class UBinggyInventoryItemDefinition;

UCLASS()
class BINGGY_API AWeaponSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// UBinggyInventoryItemDefinition for debug
	// EditInstanceOnly TODO
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Binggy|WeaponPickup")
	TSubclassOf<UBinggyInventoryItemDefinition> WeaponDefinition;

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Binggy|WeaponPickup")
	TObjectPtr<UCapsuleComponent> CollisionVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Binggy|WeaponPickup")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	bool DebugInventory(TSubclassOf<UBinggyInventoryItemDefinition> WeaponItemClass, APawn* ReceivingPawn);
};
