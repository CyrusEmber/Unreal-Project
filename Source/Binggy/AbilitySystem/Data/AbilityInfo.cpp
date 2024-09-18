// Fill out your copyright notice in the Description page of Project Settings.


#include "Binggy/AbilitySystem/Data/AbilityInfo.h"

#include "Binggy/BinggyLogChannel.h"

FBinggyAbilityInfo UAbilityInfo::GetAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound)
{
	for (const auto& AbilityInfo: AbilitiesInformation)
	{
		if (AbilityInfo.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return AbilityInfo;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogBinggy, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	
	return FBinggyAbilityInfo();
}
