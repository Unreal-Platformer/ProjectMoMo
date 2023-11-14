// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_MOMO_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);
	inline float GetMaxHP() const { return MaxHP; }
	inline float GetCurrentHP() const { return CurrentHP; }
	inline float GetHPRatio() const
	{
		// KINDA_SMALL_NUMBER = 언리얼에서 제공하는 매크로 엡실론
		return (CurrentHP < KINDA_SMALL_NUMBER ? 0.0f : (CurrentHP / MaxHP));
	}

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangedDelegate OnHPChanged;

protected:
	UPROPERTY(EditAnywhere, Category = Stat)
	float MaxHP = 100.f;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHP = 100.f;
};
