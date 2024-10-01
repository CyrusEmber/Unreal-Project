// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyGameModeBase.h"

void ABinggyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	check(CharacterClassInfo);
	check(AbilityInfoDataAsset);
}
