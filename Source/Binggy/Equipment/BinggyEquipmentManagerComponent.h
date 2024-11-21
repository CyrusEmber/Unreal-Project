// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "BinggyEquipmentManagerComponent.generated.h"

class UBinggyEquipmentManagerComponent;
struct FBinggyEquipmentList;
class UBinggyAbilitySystemComponent;
class UBinggyEquipmentInstance;
class UBinggyEquipmentDefinition;
/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct FBinggyAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FBinggyAppliedEquipmentEntry()
	{}

	FString GetDebugString() const;

private:
	friend FBinggyEquipmentList;
	friend UBinggyEquipmentManagerComponent;

	// The equipment class that got equipped
	UPROPERTY()
	TSubclassOf<UBinggyEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UBinggyEquipmentInstance> Instance = nullptr;

};

/** List of applied equipment */
USTRUCT(BlueprintType)
struct FBinggyEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FBinggyEquipmentList()
		: OwnerComponent(nullptr)
	{
	}

	FBinggyEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FBinggyAppliedEquipmentEntry, FBinggyEquipmentList>(Entries, DeltaParms, *this);
	}

	UBinggyEquipmentInstance* AddEntry(TSubclassOf<UBinggyEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UBinggyEquipmentInstance* Instance);

private:
	UBinggyAbilitySystemComponent* GetAbilitySystemComponent() const;

	friend UBinggyEquipmentManagerComponent;

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FBinggyAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FBinggyEquipmentList> : public TStructOpsTypeTraitsBase2<FBinggyEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};

/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

	UBinggyEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer);
	
public:
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UBinggyEquipmentInstance* EquipItem(TSubclassOf<UBinggyEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(UBinggyEquipmentInstance* ItemInstance);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	//~UActorComponent interface
	//virtual void EndPlay() override;
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;
	//~End of UActorComponent interface

	/** Returns the first equipped instance of a given type, or nullptr if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UBinggyEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UBinggyEquipmentInstance> InstanceType);

	/** Returns all equipped instances of a given type, or an empty array if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UBinggyEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UBinggyEquipmentInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

private:
	UPROPERTY(Replicated)
	FBinggyEquipmentList EquipmentList;
	
};
