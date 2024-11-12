// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyTabButtonBase.h"

void UBinggyTabButtonBase::SetTabLabelInfo_Implementation(const FBinggyTabDescriptor& TabLabelInfo)
{
	SetButtonText(TabLabelInfo.TabText);
}
