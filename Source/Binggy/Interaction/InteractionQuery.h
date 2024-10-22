﻿#pragma once

#include "CoreMinimal.h"
#include "InteractionQuery.generated.h"


/** The query used for interation */
USTRUCT(BlueprintType)
struct FInteractionQuery
{
	GENERATED_BODY()
	
public:
	/** The requesting pawn. */
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> RequestingAvatar;

	/** Allow us to specify a controller - does not need to match the owner of the requesting avatar. */
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AController> RequestingController;

	/** A generic UObject to shove in extra data required for the interaction */
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<UObject> OptionalObjectData;
};
