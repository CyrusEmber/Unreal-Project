// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelInfo.h"

int32 ULevelInfo::GetLevelByXP(int32 XP)
{
	int32 Level = 1;
	while (true)
	{
		// LevelUpInformation[1] = Level 1 Information
		// LevelUpInformation[2] = Level 1 Information
		if (LevelInformation.Num() - 1 <= Level) return Level;
		if (XP >= LevelInformation[Level].LevelUpRequirement)
		{
			++Level;
		}
		else
		{
			break;
		}
	}
	return Level;
}
