#pragma once
#include "GameplayEffectTypes.h"
#include "FBinggyGameplayEffectContext.Generated.h"

USTRUCT(BlueprintType)
struct FBinggyGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
public:
	bool IsCriticalHit() const { return bIsCriticalHit; };
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetImpulseDirection() const { return ImpulseDirection; }
	
	void SetCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; };
	void SetIsSuccessfulDebuff(bool bInIsDebuff) { bIsSuccessfulDebuff = bInIsDebuff; }
	void SetDebuffDamage(float InDamage) { DebuffDamage = InDamage; }
	void SetDebuffDuration(float InDuration) { DebuffDuration = InDuration; }
	void SetDebuffFrequency(float InFrequency) { DebuffFrequency = InFrequency; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	void SetImpulseDirection(FVector InImpulseDirection) { ImpulseDirection = InImpulseDirection; }
	
	FName GetHitBoneName() const { return HitBoneName; };
	void SetHitBoneName(FName InHitBoneName) { HitBoneName = InHitBoneName; };
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	virtual FBinggyGameplayEffectContext* Duplicate() const override
	{
		FBinggyGameplayEffectContext* NewContext = new FBinggyGameplayEffectContext();
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	
protected:
	UPROPERTY()
	bool bIsCriticalHit = false;

	// The hit result is replicated
	UPROPERTY()
	FName HitBoneName = FName();

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;
	
	UPROPERTY()
	float DebuffDamage = 0.f;
	
	UPROPERTY()
	float DebuffDuration = 0.f;
	
	UPROPERTY()
	float DebuffFrequency = 0.f;
	
	TSharedPtr<FGameplayTag> DamageType;

	FVector ImpulseDirection = FVector::ZeroVector;

};

template<>
struct TStructOpsTypeTraits<FBinggyGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FBinggyGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

