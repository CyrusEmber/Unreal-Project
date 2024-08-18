// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeInfo.h"

FBinggyAttributeInfo UAttributeInfo::GetAttributeInfoByTag(FGameplayTag AttributeTag, bool bLogNotFound)
{
	for (const FBinggyAttributeInfo& Info : AttributeInfoArray)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(),*GetNameSafe(this));
	}

	return FBinggyAttributeInfo();
}
