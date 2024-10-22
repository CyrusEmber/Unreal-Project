// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitCooldownChange.h"

#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::ListenForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag)
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;
	// WaitCooldownChange->UseServerCooldown = InUseServerCooldown;

	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// Bind callback whenever the effect is applied
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnActiveEffectAddedCallback);

	// Bind callback whenever the cooldown tag changed
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(WaitCooldownChange, &UWaitCooldownChange::CooldownTagChanged);
	

	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (IsValid(ASC))
	{
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);

		ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		
	}

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActiveEffectAddedCallback(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);
	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		float TimeRemaining = 0.0f;
		float Cooldown = 0.0f;
		GetCooldownRemainingForTag(CooldownTag, TimeRemaining, Cooldown);
		CooldownStart.Broadcast(TimeRemaining);
	}

	// Could Add some replication TODO:
	/*if (ASC->GetOwnerRole() == ROLE_Authority)
	{
		// Player is Server
		OnCooldownBegin.Broadcast(CooldownTag, TimeRemaining, Duration);
	}
	else if (!UseServerCooldown && SpecApplied.GetContext().GetAbilityInstance_NotReplicated())
	{
		// Client using predicted cooldown
		OnCooldownBegin.Broadcast(CooldownTag, TimeRemaining, Duration);
	}
	else if (UseServerCooldown && SpecApplied.GetContext().GetAbilityInstance_NotReplicated() == nullptr)
	{
		// Client using Server's cooldown. This is Server's corrective cooldown GE.
		OnCooldownBegin.Broadcast(CooldownTag, TimeRemaining, Duration);
	}
	else if (UseServerCooldown && SpecApplied.GetContext().GetAbilityInstance_NotReplicated())
	{
		// Client using Server's cooldown but this is predicted cooldown GE.
		// This can be useful to gray out abilities until Server's cooldown comes in.
		OnCooldownBegin.Broadcast(CooldownTag, -1.0f, -1.0f);
	}*/
}

bool UWaitCooldownChange::GetCooldownRemainingForTag(FGameplayTag InCooldownTag, float& TimeRemaining, float& CooldownDuration)
{
	if (IsValid(ASC))
	{
		TimeRemaining = 0.f;
		// CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimeRemainingList = ASC->GetActiveEffectsTimeRemaining(Query);
		// Not necessarily find one compatible query match the cooldown tag, but now each tag is correspond to a specific effect
		if (TimeRemainingList.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = TimeRemainingList[0];
			for (int32 i = 1; i < TimeRemainingList.Num(); ++i)
			{
				if (TimeRemainingList[i] > LongestTime)
				{
					LongestTime = TimeRemainingList[i];
					BestIdx = i;
				}
			}

			TimeRemaining = TimeRemainingList[BestIdx];
			// CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}

	return false;
}
