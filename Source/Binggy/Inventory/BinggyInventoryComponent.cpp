// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyInventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BinggyInventoryItemDefinition.h"
#include "BinggyInventoryItemInstance.h"
#include "InventoryFragment_ConsumableItem.h"
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

	if (OwnerComponent->GetOwner()->HasAuthority())
	{
		BroadcastChangeMessage(NewEntry, 0, NewEntry.StackCount);
	}

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

void FBinggyInventoryList::RemoveEntryByIndex(int32 Index)
{
	FBinggyInventoryEntry& Entry = Entries[Index];
	Entry.Instance = nullptr;
	Entry.StackCount = 0;
	Entry.LastObservedCount = 0;

	MarkItemDirty(Entry);

	// The server
	if (OwnerComponent->GetOwner()->HasAuthority())
	{
		BroadcastChangeMessage(Entry, 0, Entry.StackCount);
	}
	UpdateNextAvailableSlotIndex(Index);

	// TODO server
}

void FBinggyInventoryList::TryAddInstance(UBinggyInventoryItemInstance* QueryInstance)
{
	unimplemented();
	const UInventoryFragment_InventoryItem* InventoryItem = GetItemFromInstance(QueryInstance);
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

TArray<UBinggyInventoryItemInstance*> FBinggyInventoryList::TryAddItemDefinition(TSubclassOf<UBinggyInventoryItemDefinition> ItemDef, int32 StackCount)
{
	TArray<UBinggyInventoryItemInstance*> Results;
	UBinggyInventoryItemInstance* NewInstance = NewObject<UBinggyInventoryItemInstance>(OwnerComponent->GetOwner());
	NewInstance->SetItemDef(ItemDef);
	NewInstance->CurrentStack = StackCount;
	
	const UInventoryFragment_InventoryItem* InventoryItem = Cast<UInventoryFragment_InventoryItem>(GetDefault<UBinggyInventoryItemDefinition>(ItemDef)->FindFragmentByClass(UInventoryFragment_InventoryItem::StaticClass()));
	// Not likely to happen
	if (InventoryItem == nullptr)
	{
		return Results;
	}
	
	int32 MaximumStack = InventoryItem->MaximumStack;
	
	for (FBinggyInventoryEntry& Entry : Entries)
	{
		if (!Entry.Instance || Entry.Instance->GetItemDef() != ItemDef)
		{
			continue;
		}

		int CurrentStack = Entry.StackCount;
		// The slot is full
		if (CurrentStack == MaximumStack)
		{
			continue;
		}

		int32 SpaceAvailable = MaximumStack - CurrentStack;
		int32 AmountToStack = FMath::Min(SpaceAvailable, StackCount);
		Entry.StackCount += AmountToStack;
		StackCount -= AmountToStack;
		Results.Add(Entry.Instance);

		MarkItemDirty(Entry);

		// The server
		if (OwnerComponent->GetOwner()->HasAuthority())
		{
			BroadcastChangeMessage(Entry, 0, Entry.StackCount);
		}

		if (StackCount <= 0)
		{
			break;
		}
	}
	
	// Add new entry
	if (StackCount != 0)
	{
		Results.Add(AddEntry(ItemDef, StackCount));
	}
	
	return Results;
		
		/*// Fill the slot
		if (MaximumStack - CurrentStack >= StackCount)
		{
			Entry.StackCount = CurrentStack + StackCount;
			StackCount = 0;
			MarkItemDirty(Entry);
			Results.Add(Entry.Instance);
			// The server
            if (OwnerComponent->GetOwner()->HasAuthority())
            {
                BroadcastChangeMessage(Entry, 0, Entry.StackCount);
            }
			break;
		}
		// StackCount too much for the slot, continue to next slot.
		StackCount = StackCount - (MaximumStack - CurrentStack);
		Entry.StackCount = MaximumStack;
		MarkItemDirty(Entry);
		Results.Add(Entry.Instance);*/
}

const UInventoryFragment_InventoryItem* FBinggyInventoryList::GetItemFromInstance(
	UBinggyInventoryItemInstance* Instance)
{
	const UInventoryFragment_InventoryItem* InventoryItem = Cast<UInventoryFragment_InventoryItem>(Instance->FindFragmentByClass(UInventoryFragment_InventoryItem::StaticClass()));

	return InventoryItem;
}

void FBinggyInventoryList::SwapTwoInstancesByIndex(int32 Index1, int32 Index2)
{
	if (Index1 == Index2)
	{
		return;
	}
	
	FBinggyInventoryEntry& Entry1 = Entries[Index1];
	FBinggyInventoryEntry& Entry2 = Entries[Index2];
	
	// Skip two empty objects
	if (!Entry1.Instance && !Entry2.Instance)
	{
		return;
	}
	
	Entry1.Index = Index2;
	Entry2.Index = Index1;
	Entries.Swap(Index1, Index2);

	// The server
	if (OwnerComponent->GetOwner()->HasAuthority())
	{
		BroadcastChangeMessage(Entry1, 0, Entry1.StackCount);
		BroadcastChangeMessage(Entry2, 0, Entry2.StackCount);
	}

	MarkItemDirty(Entry1);
	MarkItemDirty(Entry2);
	
	UpdateNextAvailableSlotIndex(Index2);
	UpdateNextAvailableSlotIndex(Index1);
}

void FBinggyInventoryList::BroadcastChangeMessage(FBinggyInventoryEntry& Entry, int32 OldCount, int32 NewCount)
{
	FInventoryChange Message;
	//Message.InventoryOwner = OwnerComponent;
	Message.Instance = Entry.Instance;
	Message.StackCount = NewCount;
	Message.Index = Entry.Index;
	
	//Message.Delta = NewCount - OldCount;
	if (UBinggyInventoryComponent* BinggyInventoryComponent = Cast<UBinggyInventoryComponent>(OwnerComponent))
	{
		BinggyInventoryComponent->OnInventoryChange.Broadcast(Message);
	}
	
}

void FBinggyInventoryList::BroadcastInitialMessage()
{
	for (auto Entry : Entries)
	{
		// Slightly increase performance
		if (Entry.StackCount != 0)
		{
			BroadcastChangeMessage(Entry, 0, Entry.StackCount);
		}
	}
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

void FBinggyInventoryList::UpdateNextAvailableSlotIndex(int32 Index)
{
	if (Entries[Index].Instance)
	{
		return;
	}
	
	if (Index < NextAvailableSlotIndex)
	{
		NextAvailableSlotIndex = Index;
	}
}

//////////////////////////////////////////////////////////////////////
// ULyraInventoryManagerComponent

UBinggyInventoryComponent::UBinggyInventoryComponent()
{
	SetIsReplicatedByDefault(true);
	// Replicate sub object list! TODO: It should only be true for character, not AI.
	bReplicateUsingRegisteredSubObjectList = true;
}

void UBinggyInventoryComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

TArray<UBinggyInventoryItemInstance*> UBinggyInventoryComponent::AddItemDefinition(
	TSubclassOf<UBinggyInventoryItemDefinition> ItemDef, int32 StackCount)
{
	TArray<UBinggyInventoryItemInstance*> Results;
	if (ItemDef != nullptr)
	{
		Results = InventoryList.TryAddItemDefinition(ItemDef, StackCount);
		
		for (auto Result : Results)
		{
			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
			{
				AddReplicatedSubObject(Result);
			}
		}

	}
	return Results;
}

void UBinggyInventoryComponent::AddItemInstance(UBinggyInventoryItemInstance* ItemInstance)
{
	InventoryList.TryAddInstance(ItemInstance);
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
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

void UBinggyInventoryComponent::SwapTwoInstancesByIndex(int32 Index1, int32 Index2)
{
	ServerSwapTwoInstancesByIndex(Index1, Index2);
}

void UBinggyInventoryComponent::ServerSwapTwoInstancesByIndex_Implementation(int32 Index1, int32 Index2)
{
	InventoryList.SwapTwoInstancesByIndex(Index1, Index2);
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

bool UBinggyInventoryComponent::ConsumeItemsByDefinition(UBinggyInventoryItemInstance* ItemInstance,
	int32 NumToConsume, int32 Index)
{
	ServerConsumeItemsByDefinition(ItemInstance, NumToConsume, Index);
	return true;
}
 
void UBinggyInventoryComponent::ServerConsumeItemsByDefinition_Implementation(
	UBinggyInventoryItemInstance* ItemInstance, int32 NumToConsume, int32 Index)
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor || !OwningActor->HasAuthority() || !ItemInstance)
	{
		return;
	}

	//@TODO: N squared right now as there's no acceleration structure
	//@TODO: check if the TotalStack is < NumToConsume
	TSubclassOf<UBinggyInventoryItemDefinition> ItemDef = ItemInstance->GetItemDef();
	// TODO just for NumToConsume = 1 item consume change
	if (Index >= 0)
	{
		FBinggyInventoryEntry& Entry = InventoryList.Entries[Index];
		Entry.StackCount -= NumToConsume;
		if (Entry.StackCount == 0)
		{
			
			InventoryList.RemoveEntryByIndex(Index);
			return;
		}
		// TODO: Apply the effect to self 
		ApplyConsumableItemToSelf(Entry.Instance);
		InventoryList.MarkItemDirty(Entry);

		// The server TODO
		if (GetOwner()->HasAuthority())
		{
			
			InventoryList.BroadcastChangeMessage(Entry, 0, Entry.StackCount);
		}
	} else
	{
		for (FBinggyInventoryEntry& Entry : InventoryList.Entries)
		{
			if (!Entry.Instance || Entry.Instance->GetItemDef() != ItemDef)
			{
				continue;
			}
			// The CurrentStack available is always > 0 as long as the slot exists
			int StackAvailable = Entry.StackCount;
		
			int32 AmountToStack = FMath::Min(StackAvailable, NumToConsume);
			Entry.StackCount -= AmountToStack;
			NumToConsume -= AmountToStack;
		
			InventoryList.MarkItemDirty(Entry);

			// The server TODO
			if (GetOwner()->HasAuthority())
			{
				InventoryList.BroadcastChangeMessage(Entry, 0, Entry.StackCount);
			}

			if (NumToConsume <= 0)
			{
				break;
			}
		}
	}
	
	
	/*while (TotalConsumed < NumToConsume)
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
	}*/

	// TODO: return type
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

	// Register existing UBinggyInventoryItemInstance
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

void UBinggyInventoryComponent::BroadcastInitialMessage()
{
	InventoryList.BroadcastInitialMessage();
}

void UBinggyInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize the inventory list at begin play to avoid issues
	if (GetOwner()->HasAuthority())
	{
		InventoryList = FBinggyInventoryList(this, InventorySize);
	}
	
}

void UBinggyInventoryComponent::ApplyConsumableItemToSelf(UBinggyInventoryItemInstance* ConsumableItemInstance)
{
	TSubclassOf<UGameplayEffect> ConsumableItemEffectClass = Cast<UInventoryFragment_ConsumableItem>(ConsumableItemInstance->FindFragmentByClass(UInventoryFragment_ConsumableItem::StaticClass()))->ConsumableItemEffectClass;
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());

	// TODO Apply some effect to others?
	if (ASC && ConsumableItemEffectClass)
	{
		UGameplayEffect* ConsumableItemEffectInstance = NewObject<UGameplayEffect>(GetTransientPackage(), ConsumableItemEffectClass);
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		ASC->ApplyGameplayEffectToSelf(ConsumableItemEffectInstance, 0.f, EffectContext);
	}
}



