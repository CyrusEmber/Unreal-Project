// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyInventoryItemInstance.h"

#include "BinggyInventoryItemDefinition.h"
#include "Net/UnrealNetwork.h"

void UBinggyInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemDef);
}

UBinggyInventoryItemInstance::UBinggyInventoryItemInstance()
{
}

const UBinggyInventoryItemFragment* UBinggyInventoryItemInstance::FindFragmentByClass(
	TSubclassOf<UBinggyInventoryItemFragment> FragmentClass) const
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UBinggyInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}

void UBinggyInventoryItemInstance::SetItemDef(TSubclassOf<UBinggyInventoryItemDefinition> InDef)
{
	ItemDef = InDef;
}
