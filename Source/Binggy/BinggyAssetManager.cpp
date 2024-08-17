// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyAssetManager.h"
#include "AbilitySystem/BinggyGameplayTags.h"

UBinggyAssetManager& UBinggyAssetManager::Get()
{
	check(GEngine);
	UBinggyAssetManager* BinggyAssetManager = Cast<UBinggyAssetManager>(GEngine->AssetManager);
	return *BinggyAssetManager;
}

void UBinggyAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FBinggyGameplayTags::InitializeNativeGameplayTags();
}
