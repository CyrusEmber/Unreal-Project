// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildable/Buildable.h"
#include "UI/BinggyActivatableWidget.h"
#include "BinggyBuildMenu.generated.h"

/**
 * 
 */
UCLASS()
class BINGGY_API UBinggyBuildMenu : public UBinggyActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Default")
	UDataTable* Buildables;

protected:
	// UUserWidget interface
	virtual void NativePreConstruct() override;
	// End of UUserWidget interface

private:
	UPROPERTY(meta = (BindWidget))
	class UGridPanel* Container;

	int32 RowIndex;
	int32 ColumnIndex;
	
	
};
