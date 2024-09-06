// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityLibrary.generated.h"

enum class ECharacterClass : uint8;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
/**
 * 
 */
UCLASS()
class BINGGY_API UUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Called from a widget with a player controller
	UFUNCTION(BlueprintPure, Category = "UILibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "UILibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "UILibrary|CharacterAttributesDefault")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="UILibrary|CharacterAttributesDefault")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);
private:
	static void ApplyAttributes(UAbilitySystemComponent* ASC, float Level, AActor* AvatarActor, TSubclassOf<UGameplayEffect> Attributes);
};
