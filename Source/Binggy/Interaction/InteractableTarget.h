#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableTarget.generated.h"

struct FInteractionOption;
struct FInteractionQuery;

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
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, TArray<FInteractionOption>& Options) = 0;
};