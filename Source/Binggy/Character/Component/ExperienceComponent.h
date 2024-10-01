// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ExperienceComponent.generated.h"

class UGameplayEffect;
class ULevelInfo;
struct FGameplayTag;
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

	/* Attribute and skill points, create gameplay effect at runtime to modify the value, in level up */
	UFUNCTION(BlueprintCallable, Category = "Binggy|Experience")
	float GetSkillPoints() const;
	
	UFUNCTION(BlueprintCallable, Category = "Binggy|Experience")
	float GetAttributePoints() const;

	// On experience change and level change delegate binding will not be removed. It clear OnAttributePointsChanged and OnSkillPointsChanged
	UFUNCTION(BlueprintCallable, Category = "Binggy|Experience")
	void ClearPointsDelegateBinding();

	// Should be in attribute component TODO?
	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	void UpdateAttribute(const FGameplayTag& AttributeTag);

	// Use the server ASC to apply the effect
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
	

// TODO: do I need this?
public:
	UPROPERTY(BlueprintAssignable)
	FBinggyExperience_AttributeChanged OnExperienceChanged;

	// Delegate fired when the attribute value has changed. This is called on the client but the instigator may not be valid
	UPROPERTY(BlueprintAssignable)
	FBinggyExperience_AttributeChanged OnLevelChanged;
	
	UPROPERTY(BlueprintAssignable)
	FBinggyExperience_AttributeChanged OnAttributePointsChanged;
	
	UPROPERTY(BlueprintAssignable)
	FBinggyExperience_AttributeChanged OnSkillPointsChanged;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Handle experience change and level up here TODO: Level up
	virtual void HandleExperienceChanged(const FOnAttributeChangeData& Data);

	// TODO: Change the delegate type to handle 2 levels up
	virtual void HandleLevelChanged(const FOnAttributeChangeData& Data);

	virtual void HandleAttributePointsChanged(const FOnAttributeChangeData& Data);
	virtual void HandleSkillPointsChanged(const FOnAttributeChangeData& Data);
	
	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UBinggyAbilitySystemComponent> AbilitySystemComponent;

	// Health set used by this component.
	UPROPERTY()
	TObjectPtr<const UBinggyExperienceSet> ExperienceSet;


private:
	// Level up information
	UPROPERTY(EditDefaultsOnly, Category = "Binggy|Experience")
	ULevelInfo* LevelInfo;

	// Set up PreviousLevelExperience and CurrentLevelExperience
	void SetLevelExperience(const float InLevel);

	// TODO: add points based on level info, make them in one struct
	void OnLevelUp(const float AddLevel) const;
	
	// Variables relate to 
	float PreviousLevelExperience = 0.0f;
	float CurrentLevelExperience = 0.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> AttributeEffectClass;

	
	

		
};
