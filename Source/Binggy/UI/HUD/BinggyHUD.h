// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BinggyHUD.generated.h"

class UAttributeMenuWidgetController;
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
	UOverlayWidgetController* GetOverlayWidgetController(APlayerController* PC);
	UAttributeMenuWidgetController* GetAttributeMenuController(APlayerController* PC);

	//UCharacterOverlay* CharacterOverlay;

	void InitOverlay(APlayerController* PC);

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
	TObjectPtr<UBinggyUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<UBinggyUserWidget> OverlayWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;


	// Template function
	// TODO: Is it good practice to use template function?
	template <typename T>
	T* GetWidgetController(TObjectPtr<T>& WidgetController, TSubclassOf<T> WidgetControllerClass, APlayerController* PC)
	{
		check(WidgetControllerClass);
		if (WidgetController == nullptr) {
			WidgetController = NewObject<T>(this, WidgetControllerClass);
			WidgetController->SetWidgetControllerParams(PC);
		}
		return WidgetController;
	}


public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; };
};
