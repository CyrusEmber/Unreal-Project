// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyInventoryComponent.h"

#include "BinggyInventoryItemDefinition.h"
#include "BinggyInventoryItemInstance.h"
#include "InventoryFragment_InventoryItem.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


class FLifetimeProperty;
struct FReplicationFlags;
/*#include UE_INLINE_GENERATED_CPP_BY_NAME(BinggyInventoryComponent)
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Inventory_Message_StackChanged, "Inventory.Message.StackChanged");*/

FString FBinggyInventoryEntry::GetDebugString() const
{
	TSubclassOf<UBinggyInventoryItemDefinition> ItemDef;
	if (Instance != nullptr)
	{
		ItemDef = Instance->GetItemDef();
	}

	return FString::Printf(TEXT("%s (%d x %s)"), *GetNameSafe(Instance), StackCount, *GetNameSafe(ItemDef));
}

//////////////////////////////////////////////////////////////////////
// FBinggyInventoryList

TArray<UBinggyInventoryItemInstance*> FBinggyInventoryList::GetAllItems() const
{
	TArray<UBinggyInventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FBinggyInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr) //@TODO: Would prefer to not deal with this here and hide it further?
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}

void FBinggyInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FBinggyInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.StackCount, /*NewCount=*/ 0);
		Stack.LastObservedCount = 0;
	}
}

void FBinggyInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FBinggyInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ 0, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FBinggyInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		FBinggyInventoryEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.LastObservedCount, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

UBinggyInventoryItemInstance* FBinggyInventoryList::AddEntry(TSubclassOf<UBinggyInventoryItemDefinition> ItemDef,
	int32 StackCount)
{
	UBinggyInventoryItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());
	
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Add in inventory: %i"), NextAvailableSlotIndex));
	// Check if the inventory is full, TOOD will the owner not be the inventory component?
	if (UBinggyInventoryComponent* BinggyInventoryComponent = Cast<UBinggyInventoryComponent>(OwnerComponent))
	{
		if (NextAvailableSlotIndex >= BinggyInventoryComponent->InventorySize)
		{
			return Result;
		}
	}

	FBinggyInventoryEntry& NewEntry = Entries[NextAvailableSlotIndex];
	NewEntry.Instance = NewObject<UBinggyInventoryItemInstance>(OwnerComponent->GetOwner());  //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.Instance->SetItemDef(ItemDef);
	for (UBinggyInventoryItemFragment* Fragment : GetDefault<UBinggyInventoryItemDefinition>(ItemDef)->Fragments)
	{
		// TODO: now it is not defined
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.StackCount = StackCount;
	Result = NewEntry.Instance;

	//const ULyraInventoryItemDefinition* ItemCDO = GetDefault<ULyraInventoryItemDefinition>(ItemDef);
	MarkItemDirty(NewEntry);
	UpdateNextAvailableSlotIndex();

	return Result;
}

void FBinggyInventoryList::AddEntry(UBinggyInventoryItemInstance* Instance)
{
	unimplemented();
}

void FBinggyInventoryList::RemoveEntry(UBinggyInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FBinggyInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

void FBinggyInventoryList::TryAddInstance(UBinggyInventoryItemInstance* QueryInstance)
{
	const UInventoryFragment_InventoryItem* InventoryItem = GetInventoryItemFromInstance(QueryInstance);
	// Not likely to happen
	if (InventoryItem == nullptr)
	{
		return;
	}
	
	int32 MaximumStack = InventoryItem->MaximumStack;
	int32 QueryStack = QueryInstance->CurrentStack;
	
	for (int32 Index = 0; Index < Entries.Num(); ++Index)
	{
		UBinggyInventoryItemInstance* CurrentInstance = Entries[Index].Instance;
		if (CurrentInstance == QueryInstance)
		{
			int CurrentStack = CurrentInstance->CurrentStack;
			// The slot is full
			if (CurrentStack == MaximumStack)
			{
				continue;
			}
			// Fill the slot
			if (MaximumStack - CurrentStack >= QueryStack)
			{
				// TODO: private access
				CurrentInstance->CurrentStack = CurrentStack + QueryStack;
				QueryStack = 0;
				break;
			}
			// QueryStack too much for the slot, continue to next slot.
			QueryStack = QueryStack - (MaximumStack - CurrentStack);
			CurrentInstance->CurrentStack = MaximumStack;
		}
	}
	// The inventory is full
	QueryInstance->CurrentStack = QueryStack;
}

const UInventoryFragment_InventoryItem* FBinggyInventoryList::GetInventoryItemFromInstance(
	UBinggyInventoryItemInstance* Instance)
{
	const UInventoryFragment_InventoryItem* InventoryItem = Cast<UInventoryFragment_InventoryItem>(Instance->FindFragmentByClass(UInventoryFragment_InventoryItem::StaticClass()));

	return InventoryItem;
}

void FBinggyInventoryList::BroadcastChangeMessage(FBinggyInventoryEntry& Entry, int32 OldCount, int32 NewCount)
{
	FInventoryChange Message;
	//Message.InventoryOwner = OwnerComponent;
	Message.Instance = Entry.Instance;
	Message.NewCount = NewCount;
	//Message.Delta = NewCount - OldCount;
	if (UBinggyInventoryComponent* BinggyInventoryComponent = Cast<UBinggyInventoryComponent>(OwnerComponent))
	{
		BinggyInventoryComponent->OnInventoryChange.Broadcast(Message);
	}
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Broadcast: %i"), Entries.Num()));
}

void FBinggyInventoryList::UpdateNextAvailableSlotIndex()
{
	bool bFoundEmptySlot = false;
	
	for (int32 Index = NextAvailableSlotIndex + 1; Index < Entries.Num(); ++Index)
	{
		if (Entries[Index].Instance == nullptr)
		{
			NextAvailableSlotIndex = Index;
			bFoundEmptySlot = true;
			break;
		}
	}
	
	// The inventory is full
	if (!bFoundEmptySlot)
	{
		NextAvailableSlotIndex = Entries.Num();
	}
}

//////////////////////////////////////////////////////////////////////
// ULyraInventoryManagerComponent

UBinggyInventoryComponent::UBinggyInventoryComponent()
{
	SetIsReplicatedByDefault(true);
}

void UBinggyInventoryComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

UBinggyInventoryItemInstance* UBinggyInventoryComponent::AddItemDefinition(
	TSubclassOf<UBinggyInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UBinggyInventoryItemInstance* Result = nullptr;
	if (ItemDef != nullptr)
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("AddItem definition")); 
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("Replication Inventory Definition!")); 
			AddReplicatedSubObject(Result);
		}
	}
	return Result;
}

void UBinggyInventoryComponent::AddItemInstance(UBinggyInventoryItemInstance* ItemInstance)
{
	// InventoryList.AddEntry(ItemInstance);
	InventoryList.TryAddInstance(ItemInstance);
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("AddItem instance")); 
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("Replication Inventory Instance!")); 
		AddReplicatedSubObject(ItemInstance);
	}
}

void UBinggyInventoryComponent::RemoveItemInstance(UBinggyInventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

TArray<UBinggyInventoryItemInstance*> UBinggyInventoryComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

UBinggyInventoryItemInstance* UBinggyInventoryComponent::GetItemByIndex(int32 Index) const
{
	if (Index < 0 || Index >= InventoryList.GetAllItems().Num())
	{
		return nullptr;
	}
	return InventoryList.GetAllItems()[Index];
}

UBinggyInventoryItemInstance* UBinggyInventoryComponent::FindFirstItemStackByDefinition(
	TSubclassOf<UBinggyInventoryItemDefinition> ItemDef) const
{
	for (const FBinggyInventoryEntry& Entry : InventoryList.Entries)
	{
		UBinggyInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

int32 UBinggyInventoryComponent::GetTotalItemCountByDefinition(
	TSubclassOf<UBinggyInventoryItemDefinition> ItemDef) const
{
	int32 TotalCount = 0;
	for (const FBinggyInventoryEntry& Entry : InventoryList.Entries)
	{
		UBinggyInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				++TotalCount;
			}
		}
	}

	return TotalCount;
}

bool UBinggyInventoryComponent::ConsumeItemsByDefinition(TSubclassOf<UBinggyInventoryItemDefinition> ItemDef,
	int32 NumToConsume)
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor || !OwningActor->HasAuthority())
	{
		return false;
	}

	//@TODO: N squared right now as there's no acceleration structure
	int32 TotalConsumed = 0;
	while (TotalConsumed < NumToConsume)
	{
		if (UBinggyInventoryItemInstance* Instance = UBinggyInventoryComponent::FindFirstItemStackByDefinition(ItemDef))
		{
			InventoryList.RemoveEntry(Instance);
			++TotalConsumed;
		}
		else
		{
			return false;
		}
	}

	return TotalConsumed == NumToConsume;
}

bool UBinggyInventoryComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FBinggyInventoryEntry& Entry : InventoryList.Entries)
	{
		UBinggyInventoryItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UBinggyInventoryComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing ULyraInventoryItemInstance
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FBinggyInventoryEntry& Entry : InventoryList.Entries)
		{
			UBinggyInventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}



