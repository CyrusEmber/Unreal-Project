// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyHUD.h"
#include "GameFramework/PlayerController.h"
#include "UI/Foundation/BinggyActivatableMenu.h"
#include "UI/Subsystem/BinggyUIManagerSubsystem.h"
#include "CommonUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SkillMenuWidgetController.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_GAME, "UI.Layer.Game");




void ABinggyHUD::BeginPlay()
{
	Super::BeginPlay(); 
	//AddOverlayWidget();
}

//void ABinggyHUD::AddOverlayWidget()
//{
//	//APlayerController* PlayerController = GetOwningPlayerController();
//	//if (PlayerController && CharacterOverlayClass) {
//	CharacterOverlay = CreateWidget<UCharacterOverlay>(GetWorld(), CharacterOverlayClass);
//	CharacterOverlay->AddToViewport();
//
//}


void ABinggyHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;
	if (GEngine) {
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter = FVector2D(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		float SpreadScaled = CrosshairsSpreadMax * HUDPackage.CrosshairSpread;
		FVector2D Spread(0.f, 0.f);
		if (HUDPackage.CrosshairsCenter) {

			DrawCrosshairs(HUDPackage.CrosshairsCenter, ViewportCenter, Spread);
		}
		if (HUDPackage.CrosshairsLeft) {
			Spread = FVector2D(-SpreadScaled, 0.f);
			DrawCrosshairs(HUDPackage.CrosshairsLeft, ViewportCenter, Spread);
		}

		if (HUDPackage.CrosshairsRight) {
			Spread = FVector2D(SpreadScaled, 0.f);
			DrawCrosshairs(HUDPackage.CrosshairsRight, ViewportCenter, Spread);
		}

		if (HUDPackage.CrosshairsTop) {
			Spread = FVector2D(0.f, -SpreadScaled);
			DrawCrosshairs(HUDPackage.CrosshairsTop, ViewportCenter, Spread);
		}

		if (HUDPackage.CrosshairsBottom) {
			Spread = FVector2D(0.f, SpreadScaled);
			DrawCrosshairs(HUDPackage.CrosshairsBottom, ViewportCenter, Spread);
		}

		
	}

}

UOverlayWidgetController* ABinggyHUD::GetOverlayWidgetController()
{
	return GetWidgetController<UOverlayWidgetController>(OverlayWidgetController, OverlayWidgetControllerClass);
}

UAttributeMenuWidgetController* ABinggyHUD::GetAttributeMenuController()
{
	return GetWidgetController<UAttributeMenuWidgetController>(AttributeMenuWidgetController, AttributeMenuWidgetControllerClass);
}

USkillMenuWidgetController* ABinggyHUD::GetSkillMenuController()
{
	return GetWidgetController<USkillMenuWidgetController>(SkillMenuWidgetController, SkillMenuWidgetControllerClass);
}

void ABinggyHUD::InitOverlay(UBinggyAbilitySystemComponent* InASC)
{
	AbilitySystemComponent = InASC;
	checkf(OverlayWidgetClass, TEXT("CharacterOverlayClass not set"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass not set"));
	// Handle respawn
	if (OverlayWidget && OverlayWidget->IsInViewport())
	{
		UUIBlueprintLibrary::RemoveWidgetFromLayer(GetOwningPlayerController(), OverlayWidget);
	}
	/*FInputModeGameOnly InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
    PlayerController->SetInputMode(InputMode);*/

	
	OverlayWidget = Cast<UBinggyActivatableMenu>(UUIBlueprintLibrary::PushWidgetToLayerStack(GetOwningPlayerController(), TAG_UI_LAYER_GAME, OverlayWidgetClass));
	// GetOwningPlayerController()->SetInputMode(FInputModeGameOnly());
	// Otherwise the input could hit the overlay
	OverlayWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	OverlayWidget->SetWidgetController(GetOverlayWidgetController());
	
	
	/*OverlayWidget = CreateWidget<UBinggyUserWidget>(GetWorld(), OverlayWidgetClass);
	// Add to the viewport first then initialize in the blueprint.
	OverlayWidget->AddToViewport();
	
	OverlayWidgetController = GetOverlayWidgetController();
	// Could there be duplicated callbacks added?, TODO: these are set in blueprint now.
	OverlayWidget->SetWidgetController(OverlayWidgetController);*/
	
	/*OverlayWidgetController->BindCallbacksToDependencies();
	OverlayWidgetController->BroadcastInitialValue();*/

	

}


void ABinggyHUD::DrawCrosshairs(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread)
{	
	if (Texture) {
		const float TextureWidth = Texture->GetSizeX();
		const float TextureHeight = Texture->GetSizeY();
		const FVector2D TextureDrawPoint(
			ViewportCenter.X - TextureWidth / 2.f + Spread.X,
			ViewportCenter.Y - TextureHeight / 2.f + Spread.Y
		);
		DrawTexture(Texture, TextureDrawPoint.X, TextureDrawPoint.Y, TextureWidth, TextureHeight, 0.f, 0.f, 1.f, 1.f, FLinearColor::White);
	}

}
