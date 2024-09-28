// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BinggyWidgetController.h"
#include "Binggy/AbilitySystem/Data/AttributeInfo.h"
#include "AttributeMenuWidgetController.generated.h"


class UGameplayEffect;
struct FGameplayAttribute;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSigniture, const FBinggyAttributeInfo&, AttributeInfo);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class BINGGY_API UAttributeMenuWidgetController : public UBinggyWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSigniture AttributeInfoDelegate;

	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	void UnbindDelegateOnDestroy();

	// Update the attribute value when attribute change from UI
	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	void UpdateAttribute(const FGameplayTag& AttributeTag, AActor* InCharacter);
	
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	void UnbindAllDelegates();

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfoArray;
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wiget Data")
	TObjectPtr<UDataAsset> AttributeMenuWidgetDataAsset;*/
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> AttributeEffectClass;
	

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
	
	
};
