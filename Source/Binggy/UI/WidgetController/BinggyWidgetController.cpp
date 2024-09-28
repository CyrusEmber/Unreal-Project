// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyWidgetController.h"
#include "AbilitySystemComponent.h"
#include "Binggy/UtilityLibrary.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/AbilitySystem/Attributes/BinggyAttributeSet.h"
#include "Binggy/AbilitySystem/Data/AbilityInfo.h"
#include "Binggy/PlayerState/BinggyPlayerState.h"

void UBinggyWidgetController::InitializeWithAbilitySystem(UBinggyAbilitySystemComponent* InASC)
{
	// So Far so good
	// PlayerController = Cast<ABinggyPlayerController>(PS->GetPlayerController());
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Set PS name: %s"), *PS->GetName()));
	// PlayerState = Cast<ABinggyPlayerState>(PS);
	// TODO: Potential Fix on the member variable
	AbilitySystemComponent = InASC;
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Set ASC name: %s"), *AbilitySystemComponent->GetAvatarActor()->GetName()));
	
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
		FBinggyAbilityInfo Info = AbilityInfo->GetAbilityInfoByTag(UUtilityLibrary::GetAbilityTagFromSpec(AbilitySpec));
		if (Info.AbilityTag.IsValid())
		{
			Info.InputTag = UUtilityLibrary::GetInputTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	}
}
