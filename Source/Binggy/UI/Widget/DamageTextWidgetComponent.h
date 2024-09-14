// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class BINGGY_API UDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UDamageTextWidgetComponent();

	UFUNCTION(BlueprintImplementableEvent ,BlueprintCallable, Category = "DamageText")
	void SetDamageTextAndColor(float Damage, float DamageColor = 0.f);

protected:
	// Check for widget class setup
	virtual void BeginPlay() override;
	
};
