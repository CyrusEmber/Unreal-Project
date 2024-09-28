// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyHUD.h"
#include "GameFramework/PlayerController.h"
#include "Binggy/UI/Widget/BinggyUserWidget.h"
#include "Binggy/UI/WidgetController/AttributeMenuWidgetController.h"
#include "Binggy/UI/WidgetController/OverlayWidgetController.h"
#include "Binggy/UI/WidgetController/SkillMenuWidgetController.h"




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
	OverlayWidget = CreateWidget<UBinggyUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->AddToViewport();
	

	OverlayWidgetController = GetOverlayWidgetController();
	OverlayWidget->SetWidgetController(OverlayWidgetController);

	
	OverlayWidgetController->BroadcastInitialValue();
	// Could there be duplicated callbacks added?
	OverlayWidgetController->BindCallbacksToDependencies();

	OverlayWidget->AddToViewport();
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
