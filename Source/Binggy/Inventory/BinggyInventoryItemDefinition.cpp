// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyInventoryItemDefinition.h"

const UBinggyInventoryItemFragment* UBinggyInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<UBinggyInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UBinggyInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}
