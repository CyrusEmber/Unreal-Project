// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyTabListWidgetBase.h"
#include "CommonAnimatedSwitcher.h"
#include "CommonButtonBase.h"

bool UBinggyTabListWidgetBase::GetPreregisteredTabInfo(const FName TabNameId, FBinggyTabDescriptor& OutTabInfo)
{
	const FBinggyTabDescriptor* const FoundTabInfo = PreregisteredTabInfoArray.FindByPredicate([&](FBinggyTabDescriptor& TabInfo) -> bool
	{
		return TabInfo.TabId == TabNameId;
	});
	

	if (!FoundTabInfo)
	{
		return false;
	}

	OutTabInfo = *FoundTabInfo;
	return true;
}

bool UBinggyTabListWidgetBase::RegisterDynamicTab(const FBinggyTabDescriptor& TabDescriptor)
{
	// If it's hidden we just ignore it.
	if (TabDescriptor.bHidden)
	{
		return true;
	}
	// TODO: the ptr is empty
	PendingTabLabelInfoMap.Add(TabDescriptor.TabId, TabDescriptor);

	return RegisterTab(TabDescriptor.TabId, TabDescriptor.TabButtonType, TabDescriptor.CreatedTabContentWidget);
}

void UBinggyTabListWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UBinggyTabListWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	SetupTabs();
}

void UBinggyTabListWidgetBase::NativeDestruct()
{
	// TODO? Exception
	/*for (FBinggyTabDescriptor& TabInfo : PreregisteredTabInfoArray)
	{
		
		if (TabInfo.CreatedTabContentWidget)
		{
			
			TabInfo.CreatedTabContentWidget->RemoveFromParent();
			TabInfo.CreatedTabContentWidget = nullptr;
		}
	}*/
	Super::NativeDestruct();
}

void UBinggyTabListWidgetBase::HandlePreLinkedSwitcherChanged()
{
	for (const FBinggyTabDescriptor& TabInfo : PreregisteredTabInfoArray)
	{
		// Remove tab content widget from linked switcher, as it is being disassociated.
		if (TabInfo.CreatedTabContentWidget)
		{
			TabInfo.CreatedTabContentWidget->RemoveFromParent();
		}
	}
	Super::HandlePreLinkedSwitcherChanged();
}

void UBinggyTabListWidgetBase::HandlePostLinkedSwitcherChanged()
{
	if (!IsDesignTime() && GetCachedWidget().IsValid())
	{
		// Don't bother making tabs if we're in the designer or haven't been constructed yet
		SetupTabs();
	}
	Super::HandlePostLinkedSwitcherChanged();
}

void UBinggyTabListWidgetBase::HandleTabCreation_Implementation(FName TabId, UCommonButtonBase* TabButton)
{
	FBinggyTabDescriptor* TabInfoPtr = nullptr;
	
	FBinggyTabDescriptor TabInfo;
	if (GetPreregisteredTabInfo(TabId, TabInfo))
	{
		TabInfoPtr = &TabInfo;
	}
	else
	{
		TabInfoPtr = PendingTabLabelInfoMap.Find(TabId);
	}
	
	if (TabButton->GetClass()->ImplementsInterface(UBinggyTabButtonInterface::StaticClass()))
	{
		if (ensureMsgf(TabInfoPtr, TEXT("A tab button was created with id %s but no label info was specified. RegisterDynamicTab should be used over RegisterTab to provide label info."), *TabId.ToString()))
		{
			IBinggyTabButtonInterface::Execute_SetTabLabelInfo(TabButton, *TabInfoPtr);
		}
	}

	PendingTabLabelInfoMap.Remove(TabId);
}

void UBinggyTabListWidgetBase::SetupTabs()
{
	for (FBinggyTabDescriptor& TabInfo : PreregisteredTabInfoArray)
	{
		if (TabInfo.bHidden)
		{
			continue;
		}

		// If the tab content hasn't been created already, create it.
		if (!TabInfo.CreatedTabContentWidget && TabInfo.TabContentType)
		{
			TabInfo.CreatedTabContentWidget = CreateWidget<UCommonUserWidget>(GetOwningPlayer(), TabInfo.TabContentType);
			// TODO Need delegate and event
			OnTabContentCreatedNative.Broadcast(TabInfo.TabId, Cast<UCommonUserWidget>(TabInfo.CreatedTabContentWidget));
			OnTabContentCreated.Broadcast(TabInfo.TabId, Cast<UCommonUserWidget>(TabInfo.CreatedTabContentWidget));
		}

		if (UCommonAnimatedSwitcher* CurrentLinkedSwitcher = GetLinkedSwitcher())
		{
			// Add the tab content to the newly linked switcher.
			if (!CurrentLinkedSwitcher->HasChild(TabInfo.CreatedTabContentWidget))
			{
				CurrentLinkedSwitcher->AddChild(TabInfo.CreatedTabContentWidget);
			}
		}

		// If the tab is not already registered, register it.
		if (GetTabButtonBaseByID(TabInfo.TabId) == nullptr)
		{
			RegisterTab(TabInfo.TabId, TabInfo.TabButtonType, TabInfo.CreatedTabContentWidget);
		}
	}

	// Set default selected tab, would not always be the first button
	constexpr bool bSuppressClickFeedback = true;
	if (!LastActiveTabID.IsNone() && GetTabButtonBaseByID(LastActiveTabID))
	{
		SelectTabByID(LastActiveTabID, bSuppressClickFeedback);
	} else
	{
		UCommonButtonBase* FirstButton = GetTabButtonBaseByID(GetTabIdAtIndex(0));
		if (FirstButton)
		{
			SelectTabByID(GetTabIdAtIndex(0), bSuppressClickFeedback);
		}
	}
}

void UBinggyTabListWidgetBase::OnTabSelectedHandler(FName TabId)
{
	LastActiveTabID = TabId;
}
