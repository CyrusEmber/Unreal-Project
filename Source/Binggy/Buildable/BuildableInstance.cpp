// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildable/BuildableInstance.h"

#include "Net/UnrealNetwork.h"

UBuildableInstance::UBuildableInstance()
{
}

void UBuildableInstance::OnRep_Owner()
{
}

void UBuildableInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UBuildableInstance, Owner);
}


