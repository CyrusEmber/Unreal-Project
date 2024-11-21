// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/BinggyBuildMenuButtonBase.h"

#include "Actor/BinggyWorldBuildable.h"

UBinggyBuildMenuButtonBase::UBinggyBuildMenuButtonBase()
{
}

UObject* UBinggyBuildMenuButtonBase::ClassToInstagator(TSubclassOf<ABinggyWorldBuildable> BuildableClass)
{
	return (UObject*)BuildableClass;
}
