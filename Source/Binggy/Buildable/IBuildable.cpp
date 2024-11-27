#include "IBuildable.h"

#include "Inventory/IPickupable.h"

TScriptInterface<IIBuildable> UBuildableStatics::GetFirstBuildableFromActor(AActor* Actor)
{
	// If the actor is directly Buildable Actor, return that.
	TScriptInterface<IIBuildable> BuildableActor(Actor);
	if (BuildableActor)
	{
		return BuildableActor;
	}

	// If the actor isn't pickupable, it might have a component that has a pickupable interface.
	TArray<UActorComponent*> BuildableComponents = Actor ? Actor->GetComponentsByInterface(UIPickupable::StaticClass()) : TArray<UActorComponent*>();
	if (BuildableComponents.Num() > 0)
	{
		// Get first pickupable, if the user needs more sophisticated pickup distinction, will need to be solved elsewhere.
		return TScriptInterface<IIBuildable>(BuildableComponents[0]);
	}

	return TScriptInterface<IIBuildable>();
}
