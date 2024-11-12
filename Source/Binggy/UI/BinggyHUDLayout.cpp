// Fill out your copyright notice in the Description page of Project Settings.


#include "Binggy/UI/BinggyHUDLayout.h"

#include "NativeGameplayTags.h"
#include "CommonUIExtensions.h"
#include "Input/CommonUIInputTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BinggyHUDLayout)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_ACTION_ESCAPE, "UI.Action.Escape");

// TODO: handle back action
void UBinggyHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(TAG_UI_ACTION_ESCAPE), false, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleEscapeAction)));
}

// TODO use own class
void UBinggyHUDLayout::HandleEscapeAction()
{
	if (ensure(!EscapeMenuClass.IsNull()))
	{
		UCommonUIExtensions::PushStreamedContentToLayer_ForPlayer(GetOwningLocalPlayer(), TAG_UI_LAYER_MENU, EscapeMenuClass);
	}
}
