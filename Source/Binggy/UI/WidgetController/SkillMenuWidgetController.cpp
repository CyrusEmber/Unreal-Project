// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillMenuWidgetController.h"

void USkillMenuWidgetController::BroadcastInitialValue()
{
	// Super::BroadcastInitialValue();
	BroadcastAbilityInfoForAllAbilities();
}

void USkillMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
}
