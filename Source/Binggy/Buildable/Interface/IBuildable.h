#pragma once
#include "UObject/Interface.h"
#include "IBuildable.generated.h"

enum class EBuildableState : uint8;

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UIBuildable : public UInterface
{
	GENERATED_BODY()
};

/**  */
class IIBuildable
{
	GENERATED_BODY()

public:
	// TODO: Get buildable requirements
	// Get the current status of the buildable (e.g., Pending, Completed)
	virtual EBuildableState GetBuildableStatus() const = 0;

	// Set the buildable's status
	virtual void SetBuildableStatus(EBuildableState NewStatus) = 0;

	/*// Get the cost of constructing the buildable (e.g., resources needed)
	virtual int32 GetConstructionCost() const = 0;

	// Method to start construction
	virtual void StartConstruction() = 0;

	// Method to finalize the buildable's placement
	virtual void CompleteConstruction() = 0;*/
};
