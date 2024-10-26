#pragma once

#include "CoreMinimal.h"
#include "InteractionOption.h"
#include "Abilities/GameplayAbility.h"
#include "InteractableTarget.generated.h"

struct FInteractionOption;
struct FInteractionQuery;

/** Add interact options array and set InteractableTarget */
class FInteractionOptionBuilder
{
public:
	FInteractionOptionBuilder(TScriptInterface<IInteractableTarget> InterfaceTargetScope, TArray<FInteractionOption>& InteractOptions)
		: Scope(InterfaceTargetScope)
		, Options(InteractOptions)
	{
	}

	void AddInteractionOption(const FInteractionOption& Option)
	{
		FInteractionOption& OptionEntry = Options.Add_GetRef(Option);
		OptionEntry.InteractableTarget = Scope;
	}

private:
	TScriptInterface<IInteractableTarget> Scope;
	TArray<FInteractionOption>& Options;
};

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractableTarget : public UInterface
{
public:
	GENERATED_BODY()
};

class IInteractableTarget
{
	GENERATED_BODY()

public:
	// TODO: use OptionBuilder instead like lyra.
	// The function gather options based on the query, which should be implement with any class that use the interface
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) = 0;
};