// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyWidgetController.h"
#include "AbilitySystemComponent.h"
#include "Binggy/UtilityLibrary.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/AbilitySystem/Attributes/BinggyAttributeSet.h"
#include "Binggy/AbilitySystem/Data/AbilityInfo.h"

void UBinggyWidgetController::InitializeWithAbilitySystem(UBinggyAbilitySystemComponent* InASC)
{
	// So Far so good
	// TODO: Potential Fix on the member variable
	check(InASC);
	AbilitySystemComponent = InASC;
	AttributeSet = AbilitySystemComponent->GetSet<UBinggyAttributeSet>();;
}

void UBinggyWidgetController::BroadcastInitialValue()
{
}

void UBinggyWidgetController::BindCallbacksToDependencies()
{
}

TObjectPtr<UBinggyAbilitySystemComponent> UBinggyWidgetController::GetBinggyAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void UBinggyWidgetController::BroadcastAbilityInfoForAllAbilities() const
{
	// Lock the ability list to prevent changes while modifying it
	FScopedAbilityListLock ActiveScopeLock(*AbilitySystemComponent);
	for (const FGameplayAbilitySpec& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
	{
		FBinggyAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(UUtilityLibrary::GetAbilityTagFromSpec(AbilitySpec));
		if (Info.AbilityTag.IsValid())
		{
			// TODO refactoring
			Info.InputTag = UUtilityLibrary::GetInputTagFromSpec(AbilitySpec);
			Info.StatusTag = UUtilityLibrary::GetAbilityStatusTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	}
}
