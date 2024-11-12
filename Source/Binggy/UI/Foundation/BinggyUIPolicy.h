// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BinggyUIPolicy.generated.h"

class UBinggyPrimaryGameLayout;

USTRUCT()
struct FBinggyRootViewportLayoutInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UBinggyPrimaryGameLayout> RootLayout = nullptr;

	UPROPERTY(Transient)
	bool bAddedToViewport = false;

	FBinggyRootViewportLayoutInfo() {}
	FBinggyRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UBinggyPrimaryGameLayout* InRootLayout, bool bIsInViewport)
		: LocalPlayer(InLocalPlayer)
		, RootLayout(InRootLayout)
		, bAddedToViewport(bIsInViewport)
	{}

	bool operator==(const ULocalPlayer* OtherLocalPlayer) const { return LocalPlayer == OtherLocalPlayer; }
};

class UBinggyUIManagerSubsystem;
class UBinggyPrimaryGameLayout;
/**
 * Capable of changing policy at runtime, or the UBinggyPrimaryGameLayout
 */
UCLASS(Abstract, Blueprintable, Within = BinggyUIManagerSubsystem)
class BINGGY_API UBinggyUIPolicy : public UObject
{
	GENERATED_BODY()

public:
	template <typename UIPolicyClass = UBinggyUIPolicy>
	static UIPolicyClass* GetGameUIPolicyAs(const UObject* WorldContextObject)
	{
		return Cast<UIPolicyClass>(GetGameUIPolicy(WorldContextObject));
	}

	static UBinggyUIPolicy* GetGameUIPolicy(const UObject* WorldContextObject);

public:
	virtual UWorld* GetWorld() const override;
	UBinggyUIManagerSubsystem* GetOwningUIManager() const;
	UBinggyPrimaryGameLayout* GetRootLayout(const ULocalPlayer* LocalPlayer) const;

	void RequestPrimaryControl(UBinggyPrimaryGameLayout* Layout);

protected:
	void AddLayoutToViewport(ULocalPlayer* LocalPlayer, UBinggyPrimaryGameLayout* Layout);
	void RemoveLayoutFromViewport(ULocalPlayer* LocalPlayer, UBinggyPrimaryGameLayout* Layout);

	// Not implemented
	virtual void OnRootLayoutAddedToViewport(ULocalPlayer* LocalPlayer, UBinggyPrimaryGameLayout* Layout);
	virtual void OnRootLayoutRemovedFromViewport(ULocalPlayer* LocalPlayer, UBinggyPrimaryGameLayout* Layout);
	virtual void OnRootLayoutReleased(ULocalPlayer* LocalPlayer, UBinggyPrimaryGameLayout* Layout);

	// 
	void CreateLayoutWidget(ULocalPlayer* LocalPlayer);
	// Return the default LayoutClass
	TSubclassOf<UBinggyPrimaryGameLayout> GetLayoutWidgetClass(ULocalPlayer* LocalPlayer);

private:
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UBinggyPrimaryGameLayout> LayoutClass;
	
	UPROPERTY(Transient)
	TArray<FBinggyRootViewportLayoutInfo> RootViewportLayouts;

private:
	void NotifyPlayerAdded(ULocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(ULocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer);

	friend class UBinggyUIManagerSubsystem;
	
};
