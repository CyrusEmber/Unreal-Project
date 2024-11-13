// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "BinggyTabListWidgetBase.generated.h"

USTRUCT(BlueprintType)
struct FBinggyTabDescriptor
{
	GENERATED_BODY()

public:
	FBinggyTabDescriptor()
	: bHidden(false)
	, CreatedTabContentWidget(nullptr)
	{ }
	// TODO why tab is FName? Tab index?
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TabId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TabText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush IconBrush;

	// TODO set it somewhere
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bHidden;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCommonButtonBase> TabButtonType;

	//TODO NDarnell - This should become a TSoftClassPtr<>, the underlying common tab list needs to be able to handle lazy tab content construction.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> TabContentType;

	// The content ptr
	UPROPERTY(Transient)
	TObjectPtr<UWidget> CreatedTabContentWidget;
};

UINTERFACE(BlueprintType)
class UBinggyTabButtonInterface : public UInterface
{
	GENERATED_BODY()
};

class IBinggyTabButtonInterface
{
	GENERATED_BODY()

public:
	// Initialize the tab button only
	UFUNCTION(BlueprintNativeEvent, Category = "Tab Button")
	void SetTabLabelInfo(const FBinggyTabDescriptor& TabDescriptor);
};

/**
 * The hidden tab will not be constructed.
 */
UCLASS(Blueprintable, BlueprintType, Abstract, meta = (DisableNativeTick))
class BINGGY_API UBinggyTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tab List")
	bool GetPreregisteredTabInfo(const FName TabNameId, FBinggyTabDescriptor& OutTabInfo);

	/** TODO Helper method to get at all the preregistered tab infos */
	// const TArray<FBinggyTabDescriptor>& GetAllPreregisteredTabInfos() { return PreregisteredTabInfoArray; }

	// It registers tab with switcher based on the TabDescriptor TODO: not used
	/*UFUNCTION(BlueprintCallable, Category = "Tab List")
	bool RegisterDynamicTab(const FBinggyTabDescriptor& TabDescriptor);*/

	/** Delegate broadcast when a new tab is created. Allows hook ups after creation. */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTabContentCreated, FName, TabId, UCommonUserWidget*, TabWidget);
	DECLARE_EVENT_TwoParams(UBinggyTabListWidgetBase, FOnTabContentCreatedNative, FName /* TabId */, UCommonUserWidget* /* TabWidget */);

	/** Broadcasts when a new tab is created. */
	UPROPERTY(BlueprintAssignable, Category = "Tab List")
	FOnTabContentCreated OnTabContentCreated;
	FOnTabContentCreatedNative OnTabContentCreatedNative;

protected:
	// UUserWidget interface
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// End UUserWidget

	// Register tab to switcher when switcher changes
	virtual void HandlePreLinkedSwitcherChanged() override;
	virtual void HandlePostLinkedSwitcherChanged() override;

	// Handle tab creation for the tab button
	virtual void HandleTabCreation_Implementation(FName TabId, UCommonButtonBase* TabButton) override;

	
	/** The input action for inventory tab */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TabList, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle InventoryInputActionData;

	/** The input action for map tab */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TabList, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle MapInputActionData;

private:
	// Create pointer for content and bind it with switcher
	void SetupTabs();
	
	// TODO: activate the menu with a key and go straight to the specific tab

	// Callback function for OnTabSelected
	UFUNCTION()
	void OnTabSelectedHandler(FName TabId);

	FName LastActiveTabID;

	// Where I store all the tab info in the designer
	UPROPERTY(EditAnywhere, meta=(TitleProperty="TabId"), Category = TabList)
	TArray<FBinggyTabDescriptor> PreregisteredTabInfoArray;
	
	/**
	 * Stores label info for tabs that have been registered at runtime but not yet created. 
	 * Elements are removed once they are created.
	 */
	/*UPROPERTY()
	TMap<FName, FBinggyTabDescriptor> PendingTabLabelInfoMap;*/
	
};
