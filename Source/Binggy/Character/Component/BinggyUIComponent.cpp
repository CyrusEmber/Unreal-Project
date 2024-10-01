// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyUIComponent.h"

#include "Binggy/UI/Widget/BinggyUserWidget.h"
#include "Binggy/UI/WidgetController/OverlayWidgetController.h"
#include "Binggy/UI/WidgetController/SkillMenuWidgetController.h"
#include "Binggy/UI/WidgetController/AttributeMenuWidgetController.h"

// Sets default values for this component's properties
UBinggyUIComponent::UBinggyUIComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	
	AbilitySystemComponent = nullptr;
}


// Called when the game starts
void UBinggyUIComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

UOverlayWidgetController* UBinggyUIComponent::GetOverlayWidgetController()
{
	return GetWidgetController<UOverlayWidgetController>(OverlayWidgetController, OverlayWidgetControllerClass);
}

UAttributeMenuWidgetController* UBinggyUIComponent::GetAttributeMenuController()
{
	return GetWidgetController<UAttributeMenuWidgetController>(AttributeMenuWidgetController, AttributeMenuWidgetControllerClass);
}

USkillMenuWidgetController* UBinggyUIComponent::GetSkillMenuController()
{
	return GetWidgetController<USkillMenuWidgetController>(SkillMenuWidgetController, SkillMenuWidgetControllerClass);
}

void UBinggyUIComponent::InitializeWithAbilitySystem(UBinggyAbilitySystemComponent* InASC)
{
	AbilitySystemComponent = InASC;
}

void UBinggyUIComponent::UninitializeFromAbilitySystem()
{
	AbilitySystemComponent = nullptr;
}

void UBinggyUIComponent::InitOverlay()
{
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
