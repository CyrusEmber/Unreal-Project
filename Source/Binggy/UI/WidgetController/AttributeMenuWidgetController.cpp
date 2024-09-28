// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeMenuWidgetController.h"


#include "AbilitySystemBlueprintLibrary.h"
#include "Binggy/AbilitySystem/BinggyAbilitySystemComponent.h"
#include "Binggy/AbilitySystem/BinggyGameplayTags.h"
#include "Binggy/AbilitySystem/Attributes/BinggyAttributeSet.h"
#include "Binggy/AbilitySystem/Attributes/BinggyExperienceSet.h"
#include "GameFramework/PlayerState.h"


void UAttributeMenuWidgetController::BroadcastInitialValue()
{
	Super::BroadcastInitialValue();
	check(AttributeEffectClass);
	for (auto& Pair : AttributeSet->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
		/*// Debug
		UGameplayEffect* GEDebug = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Bounty")));
		GEDebug->DurationPolicy = EGameplayEffectDurationType::Instant;

		int32 Idx = GEDebug->Modifiers.Num();
		GEDebug->Modifiers.SetNum(Idx + 1);

		FGameplayModifierInfo& AttributePoints = GEDebug->Modifiers[Idx];
		AttributePoints.ModifierMagnitude = FScalableFloat(1.0f);
		AttributePoints.ModifierOp = EGameplayModOp::Additive;
		AttributePoints.Attribute = UBinggyExperienceSet::GetAttributePointsAttribute();

		AbilitySystemComponent->ApplyGameplayEffectToSelf(GEDebug, 1.0f, AbilitySystemComponent->MakeEffectContext());*/
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	for (auto& Pair : AttributeSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::UnbindDelegateOnDestroy()
{
	AttributeInfoDelegate.Clear();
}

// Check if we have enough attribute point ?
void UAttributeMenuWidgetController::UpdateAttribute(const FGameplayTag& AttributeTag, AActor* InCharacter)
{
	/*const FBinggyGameplayTags& GameplayTags = FBinggyGameplayTags::Get();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Send to actor name: %s"), *AbilitySystemComponent->GetAvatarActor()->GetName()));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Current actor name: %s"), *InCharacter->GetName()));
	// Handle attribute change
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Send to owner get pawn name: %s"), *Cast<APlayerState>(AbilitySystemComponent->GetOwner())->GetPawn()->GetName()));
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Cast<APlayerState>(AbilitySystemComponent->GetOwner())->GetPawn(), Payload.EventTag, Payload);
	
	// Handle attribute point change
	Payload.EventTag = GameplayTags.Attributes_Experience_AttributePoints;
	Payload.EventMagnitude = -1.f;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Cast<APlayerState>(AbilitySystemComponent->GetOwner())->GetPawn(), Payload.EventTag, Payload);*/

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	// TODO Specify the level
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(AttributeEffectClass, 1, EffectContext);
	FBinggyGameplayTags GameplayTags = FBinggyGameplayTags::Get();
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Experience_AttributePoints, -1.f);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AttributeTag, 1.f);
	
	// Apply the effect to the character
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	// ServerUpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	// TODO Specify the level
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(AttributeEffectClass, 1, EffectContext);
	FBinggyGameplayTags GameplayTags = FBinggyGameplayTags::Get();
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Experience_AttributePoints, -1.f);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AttributeTag, 1.f);
	
	// Apply the effect to the character
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	/*const FBinggyGameplayTags& GameplayTags = FBinggyGameplayTags::Get();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Server RPC")));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Send to actor name: %s"), *AbilitySystemComponent->GetAvatarActor()->GetName()));
	// Handle attribute change
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Send to owner name: %s"), *AbilitySystemComponent->GetOwner()->GetName()));
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AbilitySystemComponent->GetAvatarActor(), Payload.EventTag, Payload);
	
	// Handle attribute point change
	Payload.EventTag = GameplayTags.Attributes_Experience_AttributePoints;
	Payload.EventMagnitude = -1.f;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AbilitySystemComponent->GetAvatarActor(), Payload.EventTag, Payload);*/
}

void UAttributeMenuWidgetController::UnbindAllDelegates()
{
	for (auto& Pair : AttributeSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).RemoveAll(this);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FBinggyAttributeInfo Info = AttributeInfoArray->GetAttributeInfoByTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
