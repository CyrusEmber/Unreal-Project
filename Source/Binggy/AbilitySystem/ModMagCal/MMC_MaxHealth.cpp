// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC_MaxHealth.h"
#include "AbilitySystem/Attributes/BinggyAttributeSet.h"
#include "AbilitySystem/Attributes/BinggyExperienceSet.h"
#include "Interface/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UBinggyAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	LevelDef.AttributeToCapture = UBinggyExperienceSet::GetLevelAttribute();
	LevelDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	LevelDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
	RelevantAttributesToCapture.Add(LevelDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	// If enabling this for a custom calculation, be sure that the external delegate is sourced from something guaranteed to be on the client to avoid
	//*timing issues.As an example, binding to a delegate on a GameState is potentially unreliable, as the client reference to that actor may not be
	//	* available during registration.

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	float Level = 0.f;
	if (!GetCapturedAttributeMagnitude(LevelDef, Spec, EvaluationParameters, Level))
	{
		// Set a default value if the Level attribute is not present
		Level = 0.f; // or any other default value you want
	}
	Level = FMath::Max<float>(Level, 0.f);

	/*ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();*/
	
	// TODO: Base value or get from interface?
	return 50.0f + 10.f * Vigor + 10.f * Level;
}
