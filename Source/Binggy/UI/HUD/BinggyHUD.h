// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/BinggyAbilitySystemComponent.h"
#include "GameFramework/HUD.h"
#include "BinggyHUD.generated.h"

class UBinggyActivatableMenu;
class USkillMenuWidgetController;
class UAttributeMenuWidgetController;
class USkillMenuWidgetController;
class UTexture2D;
class UCharacterOverlay;
class UBinggyUserWidget;
class UOverlayWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;

USTRUCT(BlueprintType)
struct FHUDPackage {
	GENERATED_BODY()
public:
	UTexture2D* CrosshairsCenter;
	UTexture2D* CrosshairsLeft;
	UTexture2D* CrosshairsRight;
	UTexture2D* CrosshairsTop;
	UTexture2D* CrosshairsBottom;
	float CrosshairSpread;
};

/**
 * 
 */

UCLASS()
class BINGGY_API ABinggyHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;
	

	/* Get Widget Controller if it is not set yet, set the widget controller */
	UOverlayWidgetController* GetOverlayWidgetController();
	UAttributeMenuWidgetController* GetAttributeMenuController();
	USkillMenuWidgetController* GetSkillMenuController();

	//UCharacterOverlay* CharacterOverlay;

	void InitOverlay(UBinggyAbilitySystemComponent* InASC);

protected:
	virtual void BeginPlay() override;
	/*void AddOverlayWidget();*/

private:
	FHUDPackage HUDPackage;
	void DrawCrosshairs(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread);

	UPROPERTY(EditAnywhere)
	float CrosshairsSpreadMax = 16.f;

	//UPROPERTY(EditAnywhere, Category = "Player Stats")
	//TSubclassOf<UUserWidget> CharacterOverlayClass;
	
	// Set WidgetController
	UPROPERTY()
	TObjectPtr<UBinggyActivatableMenu> OverlayWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UBinggyActivatableMenu> OverlayWidgetClass;
	
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

	// Used when initialize the WidgetController
	TObjectPtr<UBinggyAbilitySystemComponent> AbilitySystemComponent;


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


public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; };
};
