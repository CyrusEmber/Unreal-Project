#pragma once
#include "GameplayEffectTypes.h"
#include "FBinggyGameplayEffectContext.Generated.h"

USTRUCT(BlueprintType)
struct FBinggyGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
public:
	bool IsCriticalHit() const { return bIsCriticalHit; };
	void SetCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; };
	
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

