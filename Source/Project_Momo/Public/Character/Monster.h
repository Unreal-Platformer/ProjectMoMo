// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

class UCharacterStatComponent;
class UAnimMontage;

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName = "Alive"),
	EDP_DeathFront UMETA(DisplayName = "DeathFront"),
	EDP_DeathBack UMETA(DisplayName = "DeathBack"),
	EDP_DeathRight UMETA(DisplayName = "DeathRight"),
	EDP_DeathLeft UMETA(DisplayName = "DeathLeft"),
};

UCLASS()
class PROJECT_MOMO_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:

public:
	AMonster();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void Die();

protected:
	UPROPERTY(VIsibleAnywhere, Category = Stat)
	UCharacterStatComponent* CharacterStat;

	UPROPERTY(VIsibleAnywhere, Category = Montage)
	UAnimMontage* DeathMontage;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;
};
