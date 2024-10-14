// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BinggyUIComponent.generated.h"


class UBinggyUserWidget;
class UBinggyAbilitySystemComponent;
class USkillMenuWidgetController;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;


// Not Used class, the widget controller is stored in the HUD
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BINGGY_API UBinggyUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBinggyUIComponent();

	/* Get Widget Controller if it is not set yet, set the widget controller */
	UFUNCTION(BlueprintCallable)
	UOverlayWidgetController* GetOverlayWidgetController();
	UFUNCTION(BlueprintCallable)
	UAttributeMenuWidgetController* GetAttributeMenuController();
	UFUNCTION(BlueprintCallable)
	USkillMenuWidgetController* GetSkillMenuController();

	UFUNCTION(BlueprintPure, Category = "Binggy")
	static UBinggyUIComponent* FindUIComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UBinggyUIComponent>() : nullptr); }

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "Binggy|Health")
	void InitializeWithAbilitySystem(UBinggyAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category = "Binggy|Health")
	void UninitializeFromAbilitySystem();

	// Add overlay to viewport
	void InitOverlay();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UBinggyUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<UBinggyUserWidget> OverlayWidgetClass;

	// Controller class
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USkillMenuWidgetController> SkillMenuWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<USkillMenuWidgetController> SkillMenuWidgetControllerClass;

		
	UBinggyAbilitySystemComponent* AbilitySystemComponent;

	// Template function
	// TODO: Is it good practice to use template function?
	template <typename T>
	T* GetWidgetController(TObjectPtr<T>& WidgetController, TSubclassOf<T> WidgetControllerClass)
	{
		check(WidgetControllerClass);
		if (WidgetController == nullptr) {
			WidgetController = NewObject<T>(this, WidgetControllerClass);
			WidgetController->InitializeWithAbilitySystem(AbilitySystemComponent);
		}
		return WidgetController;
	}

	

		
};

