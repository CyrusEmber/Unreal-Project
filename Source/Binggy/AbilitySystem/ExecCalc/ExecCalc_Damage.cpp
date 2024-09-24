// Fill out your copyright notice in the Description page of Project Settings.


#include "Binggy/AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "Binggy/UtilityLibrary.h"
#include "Binggy/AbilitySystem/BinggyGameplayTags.h"
#include "Binggy/AbilitySystem/Attributes/BinggyAttributeSet.h"

struct BinggyDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamage);
	BinggyDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBinggyAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBinggyAttributeSet, CriticalChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBinggyAttributeSet, CriticalDamage, Source, false);
	}
};

static const BinggyDamageStatics& DamageStatics()
{
	static BinggyDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	// Get Damage Set by Caller Magnitude
	float Damage = 0.f;
	for (auto& Pair : FBinggyGameplayTags::Get().DamageTypesToResistanceTypes)
	{
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);
		Damage += DamageTypeValue;
	}

	// CriticalChance and CriticalDamage are from source character
	// CriticalChance
	float CriticalChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalChanceDef,  EvaluationParameters, CriticalChance);
	CriticalChance = FMath::Max<float>(0.f, CriticalChance);

	const bool bCriticalHit = FMath::RandRange(0, 100) < CriticalChance;
	
	// Set critical hit to effect context
	UUtilityLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	if (bCriticalHit)
	{
		// CriticalDamage
		float CriticalDamage = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalDamageDef,  EvaluationParameters, CriticalDamage);
		CriticalDamage = FMath::Max<float>(0.f, CriticalDamage);
		
		Damage *= (1 + CriticalDamage / 100);
	}

	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Damage: %f"), Damage));
	const FGameplayModifierEvaluatedData EvaluatedData(UBinggyAttributeSet::GetDamageAttribute(), EGameplayModOp::Override, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}