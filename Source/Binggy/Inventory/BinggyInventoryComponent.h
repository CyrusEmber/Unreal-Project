// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "BinggyInventoryComponent.generated.h"


class UInventoryFragment_InventoryItem;
class UBinggyInventoryItemDefinition;
class UBinggyInventoryComponent;
class UBinggyInventoryItemInstance;
class UObject;
struct FFrame;
struct FBinggyInventoryList;
struct FBinggyInventoryEntry;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;



/** A message when an item is added to the inventory */
USTRUCT(BlueprintType)
struct FInventoryChange
{
	GENERATED_BODY()

	//@TODO: Tag based names+owning actors for inventories instead of directly exposing the component?
	/*UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<UActorComponent> InventoryOwner = nullptr;*/

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UBinggyInventoryItemInstance> Instance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 NewCount = 0;

	/*UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 Delta = 0;*/

	// Index in the UI
	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 Index = 0;
};

/** A single entry in an inventory */
USTRUCT(BlueprintType)
struct FBinggyInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FBinggyInventoryEntry()
	{}

	FString GetDebugString() const;

private:
	friend FBinggyInventoryList;
	friend UBinggyInventoryComponent;

	UPROPERTY()
	TObjectPtr<UBinggyInventoryItemInstance> Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	// TODO? What for?
	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;

	UPROPERTY()
	int32 Index = INDEX_NONE;
};


/** List of inventory items */
USTRUCT(BlueprintType)
struct FBinggyInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FBinggyInventoryList()
		: OwnerComponent(nullptr)
	{
	}

	FBinggyInventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}
	// TODO saves
	FBinggyInventoryList(UActorComponent* InOwnerComponent, int32 InventorySize)
	: OwnerComponent(InOwnerComponent)
	{
		Entries.SetNum(InventorySize);
		
		// Set the Index property to match the array position
		for (int32 i = 0; i < InventorySize; ++i)
		{
			Entries[i].Index = i;
			// TODO how to make the array dirty?
		}
		MarkArrayDirty();
	}

	TArray<UBinggyInventoryItemInstance*> GetAllItems() const;

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FBinggyInventoryEntry, FBinggyInventoryList>(Entries, DeltaParms, *this);
	}

	// Add to the NextAvailableSlotIndex
	UBinggyInventoryItemInstance* AddEntry(TSubclassOf<UBinggyInventoryItemDefinition> ItemDef, int32 StackCount);
	void AddEntry(UBinggyInventoryItemInstance* Instance);

	void RemoveEntry(UBinggyInventoryItemInstance* Instance);

	

private:
	void BroadcastChangeMessage(FBinggyInventoryEntry& Entry, int32 OldCount, int32 NewCount);

	// Try to add instance to the array, if there is any duplicated items, add to its stack until hit maximum stack.
	void TryAddInstance(UBinggyInventoryItemInstance* QueryInstance);
	const UInventoryFragment_InventoryItem* GetInventoryItemFromInstance(UBinggyInventoryItemInstance* Instance);

	// TODO Initialize from saves and constructor
	int32 NextAvailableSlotIndex = 0;
	void UpdateNextAvailableSlotIndex();

private:
	friend UBinggyInventoryComponent;

private:
	// Replicated list of items
	UPROPERTY()
	TArray<FBinggyInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FBinggyInventoryList> : public TStructOpsTypeTraitsBase2<FBinggyInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChange, FInventoryChange, InventoryChange);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BINGGY_API UBinggyInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBinggyInventoryComponent();

	/*UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	bool CanAddItemDefinition(TSubclassOf<ULyraInventoryItemDefinition> ItemDef, int32 StackCount = 1);*/

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UBinggyInventoryItemInstance* AddItemDefinition(TSubclassOf<UBinggyInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddItemInstance(UBinggyInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void RemoveItemInstance(UBinggyInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure=false)
	TArray<UBinggyInventoryItemInstance*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure=false)
	UBinggyInventoryItemInstance* GetItemByIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure)
	UBinggyInventoryItemInstance* FindFirstItemStackByDefinition(TSubclassOf<UBinggyInventoryItemDefinition> ItemDef) const;

	int32 GetTotalItemCountByDefinition(TSubclassOf<UBinggyInventoryItemDefinition> ItemDef) const;
	bool ConsumeItemsByDefinition(TSubclassOf<UBinggyInventoryItemDefinition> ItemDef, int32 NumToConsume);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	//~End of UObject interface

	UPROPERTY(BlueprintAssignable)
	FOnInventoryChange OnInventoryChange;

private:
	// TODO Saved
	int32 InventorySize = 16;

	friend FBinggyInventoryList;
	
	UPROPERTY(Replicated)
	FBinggyInventoryList InventoryList = FBinggyInventoryList(this, InventorySize);
};
