// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyInventoryItemInstance.h"

#include "Net/UnrealNetwork.h"

void UBinggyInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemDef);
}

UBinggyInventoryItemInstance::UBinggyInventoryItemInstance()
{
}

void UBinggyInventoryItemInstance::SetItemDef(TSubclassOf<UBinggyInventoryItemDefinition> InDef)
{
	ItemDef = InDef;
}
