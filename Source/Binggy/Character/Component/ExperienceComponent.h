// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExperienceComponent.generated.h"

struct FOnAttributeChangeData;
class UBinggyExperienceSet;
class UBinggyAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBinggyExperience_AttributeChanged, float, NewValue);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BINGGY_API UExperienceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExperienceComponent();

	UFUNCTION(BlueprintPure, Category = "Binggy|Experience")
	static UExperienceComponent* FindExperienceComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UExperienceComponent>() : nullptr); }

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "Binggy|Health")
	void InitializeWithAbilitySystem(UBinggyAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category = "Binggy|Health")
	void UninitializeFromAbilitySystem();

	// Returns the current experience. TODO: max experience
	UFUNCTION(BlueprintCallable, Category = "Binggy|Experience")
	float GetExperience() const;

	// Returns the current level. TODO: Add functionality of set LastLevelExperience
	UFUNCTION(BlueprintCallable, Category = "Binggy|Experience")
	float GetLevel() const;

	// Returns the current experience ratio in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "Binggy|Experience")
	float GetExperienceNormalized() const;

// TODO: do I need this?
public:
	UPROPERTY(BlueprintAssignable)
	FBinggyExperience_AttributeChanged OnExperienceChanged;

	// Delegate fired when the max health value has changed. This is called on the client but the instigator may not be valid
	UPROPERTY(BlueprintAssignable)
	FBinggyExperience_AttributeChanged OnLevelChanged;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Handle experience change and level up here TODO: Level up
	virtual void HandleExperienceChanged(const FOnAttributeChangeData& Data);
	virtual void HandleLevelChanged(const FOnAttributeChangeData& Data);
	
	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UBinggyAbilitySystemComponent> AbilitySystemComponent;

	// Health set used by this component.
	UPROPERTY()
	TObjectPtr<const UBinggyExperienceSet> ExperienceSet;

private:
	// Variables relate to 

	float PreviousLevelExperience = 0.0f;
	float CurrentLevelExperience = 300.f;

	
	

		
};
