// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyUIManagerSubsystem.h"

#include "Binggy/UI/Foundation/BinggyUIPolicy.h"

void UBinggyUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Create the default policy
	if (!CurrentPolicy && !DefaultUIPolicyClass.IsNull())
	{
		TSubclassOf<UBinggyUIPolicy> PolicyClass = DefaultUIPolicyClass.LoadSynchronous();
		SwitchToPolicy(NewObject<UBinggyUIPolicy>(this, PolicyClass));
	}
}

void UBinggyUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();

	SwitchToPolicy(nullptr);
}

bool UBinggyUIManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> ChildClasses;
		GetDerivedClasses(GetClass(), ChildClasses, false);

		// Only create an instance if there is no override implementation defined elsewhere
		return ChildClasses.Num() == 0;
	}

	return false;
	
}

void UBinggyUIManagerSubsystem::NotifyPlayerAdded(ULocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer) && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerAdded(LocalPlayer);
	}
}

void UBinggyUIManagerSubsystem::NotifyPlayerRemoved(ULocalPlayer* LocalPlayer)
{
	if (LocalPlayer && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerRemoved(LocalPlayer);
	}
}

void UBinggyUIManagerSubsystem::NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer)
{
	if (LocalPlayer && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerDestroyed(LocalPlayer);
	}
}

void UBinggyUIManagerSubsystem::SwitchToPolicy(UBinggyUIPolicy* InPolicy)
{
	if (CurrentPolicy != InPolicy)
	{
		CurrentPolicy = InPolicy;
	}
}

ULocalPlayer* UBinggyUIManagerSubsystem::GetFirstLocalPlayer()
{
	const TArray<ULocalPlayer*>& LocalPlayers = GetGameInstance()->GetLocalPlayers();
	if (LocalPlayers.Num() > 0)
	{
		return LocalPlayers[0];
	}
	return nullptr;
}
