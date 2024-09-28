// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BinggyWidgetController.generated.h"


class UBinggyAttributeSet;
class ABinggyPlayerController;
class ABinggyPlayerState;
class UAbilityInfo;
class UBinggyAbilitySystemComponent;
class UAbilitySystemComponent;
class UAttributeSet;

/*USTRUCT(BlueprintType)
struct FWidgetControllerParams {
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet;
};*/

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FBinggyAbilityInfo&, Info);
UCLASS()
class BINGGY_API UBinggyWidgetController : public UObject
{
	GENERATED_BODY()

public:
	// Use player state to set the controller because player state is replicated across all networks. This do sets the player controller but locally, so it has some problem when broadcasting using playercontroller
	UFUNCTION(BlueprintCallable)
	void InitializeWithAbilitySystem(UBinggyAbilitySystemComponent* InASC);
	
	// Called when initialize the HUD or blueprint
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValue();
	
	// Called when initialize the HUD or blueprint
	UFUNCTION(BlueprintCallable)
	virtual void BindCallbacksToDependencies();

	TObjectPtr<UBinggyAbilitySystemComponent> GetBinggyAbilitySystemComponent() const;

	
	// TODO: make it on changed
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FAbilityInfoSignature AbilityInfoDelegate;

protected:
	// Basic pointer
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<ABinggyPlayerController> PlayerController;
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<ABinggyPlayerState> PlayerState;
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UBinggyAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<const UBinggyAttributeSet> AttributeSet;

	// Information data
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	// Broadcast the ability info of all the abilities, TODO where is the best class to put it?
	// Get the information of each ability and broadcast them to the widget.
	void BroadcastAbilityInfoForAllAbilities() const;


	

	// Data
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;*/
};
