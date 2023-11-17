// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnLifePointIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLifePointChangedDelegate);

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
	void SetLifePoint(float NewLifePoint);
	inline float GetMaxLifePoint() const { return MaxLifePoint; }
	inline float GetCurrentLifePoint() const { return CurrentLifePoint; }
	inline float GetLifePointRatio() const
	{
		// KINDA_SMALL_NUMBER = 언리얼에서 제공하는 매크로 엡실론
		return (CurrentLifePoint < KINDA_SMALL_NUMBER ? 0.0f : (CurrentLifePoint / MaxLifePoint));
	}

	FOnLifePointIsZeroDelegate OnLifePointIsZero;
	FOnLifePointChangedDelegate OnLifePointChanged;

protected:
	UPROPERTY(EditAnywhere, Category = Stat)
	float MaxLifePoint = 100.f;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLifePoint = 100.f;
};
