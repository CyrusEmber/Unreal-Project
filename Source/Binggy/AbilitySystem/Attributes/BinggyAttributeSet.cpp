// Fill out your copyright notice in the Description page of Project Settings.


#include "BinggyAttributeSet.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "UtilityLibrary.h"
#include "AbilitySystem/BinggyGameplayTags.h"
#include "PlayerController/BinggyPlayerController.h"
#include "GameFramework/Character.h"

UBinggyAttributeSet::UBinggyAttributeSet()
{
	FBinggyGameplayTags GameplayTags = FBinggyGameplayTags::Get();
	/* Primary */
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	/* Secondary */
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthGeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaGeneration, GetManaRegenerationAttribute);

	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalDamage, GetCriticalDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalChance, GetCriticalChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_PhysicalDamage, GetPhysicalDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MagicalDamage, GetMagicalDamageAttribute);

	// Vital Test health
	TagsToAttributes.Add(GameplayTags.Attributes_Vital_Health, GetHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Vital_Mana, GetManaAttribute);

	// Health Component initialization
	HealthBeforeAttributeChange = 0.f;
	MaxHealthBeforeAttributeChange = 0.f;
	bOutOfHealth = false;
}

void UBinggyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Respond to the change even if the value is unchanged
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBinggyAttributeSet, CriticalDamage, COND_None, REPNOTIFY_Always);
	

}

// Although the value is clamped, it still changed somehow
void UBinggyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	/*if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}*/


}

void UBinggyAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (bOutOfHealth && (GetHealth() > 0.0f))
	{
		bOutOfHealth = false;
	}
}

bool UBinggyAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}
	HealthBeforeAttributeChange = GetHealth();
	MaxHealthBeforeAttributeChange = GetMaxHealth();
	return true;
	
}

void UBinggyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FEffectProperties Props;
	SetEffectProperty(Data, Props);

	// Remove all damage or gameplay effects, TODO: refactor and remove even debuff without damage. Should use query
	if (bOutOfHealth)
	{
		// Owing character
		// Props.TargetASC->GetActiveEffects()
		Props.TargetASC->RemoveActiveGameplayEffectBySourceEffect(Data.EffectSpec.Def->GetClass(), nullptr);
		return;
	}

	// Extract Information
	AActor* Instigator = Props.EffectContextHandle.GetOriginalInstigator();
	AActor* Causer = Props.EffectContextHandle.GetEffectCauser();

	// GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Yellow, FString::Printf(TEXT("Character: %s, Health: %f"), *Props.TargetAvatarActor->GetName(), GetHealth()));
	// Ensure Health and Mana do not go below 0 or above their max values
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
	
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		HandleDamage(Props);
	}
	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
    	{
    		// Convert into +Health and then clamp
			// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Heal Health: %f"), GetHealth()));
    		SetHealth(FMath::Clamp(GetHealth() + GetHealing(), 0, GetMaxHealth()));
    		SetHealing(0.0f);
			
    	}
    	
	// Normally Max health does not change
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		// Clamp current health
		if (GetHealth() > GetMaxHealth())
		{
			SetHealth(GetMaxHealth());
		}
		// Notify on any requested max health changes
		// OnMaxHealthChanged.Broadcast(GetMaxHealth());
	}
	
	// If health has actually changed activate callbacks
	if (GetHealth() != HealthBeforeAttributeChange)
	{
		OnHealthChanged.Broadcast(GetHealth());
	}

	// TODO: !bOutOfHealth 
	if ((GetHealth() <= 0.0f) && !bOutOfHealth)
	{
		OnOutOfHealth.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, HealthBeforeAttributeChange, GetHealth());
		// Check health again in case an event above changed it.
		bOutOfHealth = true;
		
		/*ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
		if (CombatInterface)
		{
			CombatInterface->Die();
		}*/
	}

	

	
	
}

void UBinggyAttributeSet::SetEffectProperty(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid()) {
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr) {
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor)) {
				Props.SourceController = Pawn->GetController();
			}
		}
		// Bugged FIXME
		if (Props.SourceController) {
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	// Target is the owner
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()) {
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}

}

void UBinggyAttributeSet::ShowFloatingText(const FEffectProperties& Props, float InDamage, bool bIsCriticalHit) const
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		// UGameplayStatics::GetPlayerController(Props.SourceCharacter, 0) only find the controller where the function is executed: server
		if(ABinggyPlayerController* PC = Cast<ABinggyPlayerController>(Props.SourceCharacter->GetController()))
		{
			// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Controller: %s"), *Props.SourceCharacter->GetName()));
			// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Controller: %s"), *PC->GetName()));
			PC->ShowDamageNumber(InDamage, Props.TargetCharacter, bIsCriticalHit);
		}
	}
}

void UBinggyAttributeSet::Debuff(const FEffectProperties& Props)
{
	if (UUtilityLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
	{
		const FBinggyGameplayTags& GameplayTags = FBinggyGameplayTags::Get();
		FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
		EffectContext.AddSourceObject(Props.SourceAvatarActor);
		const FGameplayTag DamageType = UUtilityLibrary::GetDamageType(Props.EffectContextHandle);
		const float DebuffDamage = UUtilityLibrary::GetDebuffDamage(Props.EffectContextHandle);
		const float DebuffDuration = UUtilityLibrary::GetDebuffDuration(Props.EffectContextHandle);
		const float DebuffFrequency = UUtilityLibrary::GetDebuffFrequency(Props.EffectContextHandle);
		FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
		UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));
		Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
		Effect->Period = DebuffFrequency;
		Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
		
		Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
		Effect->StackLimitCount = 2;

		int32 Idx = Effect->Modifiers.Num();
		Effect->Modifiers.SetNum(Idx + 1);

		FGameplayModifierInfo& InfoDamage = Effect->Modifiers[Idx];
		InfoDamage.ModifierMagnitude = FScalableFloat(DebuffDamage);
		InfoDamage.ModifierOp = EGameplayModOp::Additive;
		InfoDamage.Attribute = GetDamageAttribute();
		
		FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f);
		
		MutableSpec->DynamicGrantedTags.AddTag(GameplayTags.DamageTypesToDebuffs[DamageType]);
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Hit Location: %s"), *GameplayTags.DamageTypesToDebuffs[DamageType].GetTagName().ToString()));
		// TODO: we could change the level, need test on the granted tags
		// Props.TargetASC->AddLooseGameplayTag(GameplayTags.DamageTypesToDebuffs[DamageType]);
		Props.SourceASC->ApplyGameplayEffectSpecToTarget(*MutableSpec, Props.TargetASC);
	}
}

void UBinggyAttributeSet::HandleDamage(const FEffectProperties& Props)
{
	// Meta attribute for damage
	const float LocalIncomingDamage = GetDamage();

	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			
		// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Character: %s, Health: %f"), *Props.TargetAvatarActor->GetName(), GetHealth()));
		const bool bFatal = NewHealth <= 0.f;
			

		if (!bFatal)
		{
			// TODO
		}
		// Show the damage text when damage > 0
		// UUtilityLibrary::IsCriticalHit(Props.EffectContextHandle)
		ShowFloatingText(Props, LocalIncomingDamage, UUtilityLibrary::IsCriticalHit(Props.EffectContextHandle));
			
		// Get Bone name and apply impulse according to damage
		Props.EffectContextHandle.GetHitResult()->BoneName;
			
		SetDamage(0.f);
	}
	
	// Success debuff effect
	Debuff(Props);
}

void UBinggyAttributeSet::OnRep_Health(FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyAttributeSet, Health, OldValue);
	const float CurrentHealth = GetHealth();
	const float EstimatedMagnitude = CurrentHealth - OldValue.GetCurrentValue();

	OnHealthChanged.Broadcast(CurrentHealth);
	
	if (CurrentHealth <= 0.0f)
	{
		// TODO: this is working twice, this is for the client ASC to broadcast
		OnOutOfHealth.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentHealth);
	}

	bOutOfHealth = (CurrentHealth <= 0.0f);
}

void UBinggyAttributeSet::OnRep_MaxHealth(FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyAttributeSet, MaxHealth, OldValue);

	// OnMaxHealthChanged.Broadcast(GetMaxHealth());
}

void UBinggyAttributeSet::OnRep_Mana(FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyAttributeSet, Mana, OldValue);
}

void UBinggyAttributeSet::OnRep_MaxMana(FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyAttributeSet, MaxMana, OldValue);
}

void UBinggyAttributeSet::OnRep_Strength(FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyAttributeSet, Strength, OldValue);
}

void UBinggyAttributeSet::OnRep_Intelligence(FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyAttributeSet, Intelligence, OldValue);
}

void UBinggyAttributeSet::OnRep_Vigor(FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyAttributeSet, Vigor, OldValue);
}

void UBinggyAttributeSet::OnRep_HealthRegeneration(FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyAttributeSet, HealthRegeneration, OldValue);
}

void UBinggyAttributeSet::OnRep_ManaRegeneration(FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBinggyAttributeSet, ManaRegeneration, OldValue);
}

void UBinggyAttributeSet::OnRep_AttributePoints(FGameplayAttributeData& OldValue) const
{
}

void UBinggyAttributeSet::OnRep_SkillPoints(FGameplayAttributeData& OldValue) const
{
}


