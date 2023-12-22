// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Monster.generated.h"

class UPawnSensingComponent;
class UAnimMontage;
class AAIController;

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
class PROJECT_MOMO_API AMonster : public ABaseCharacter
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
	virtual void Die() override;
	void Attack();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool CanAttack();
	void PlayAttackMontage();
	bool InTargetRange(AActor* Target, const double Radius) const;
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void PatrolTimerFinished();
	void CheckPatrolTarget();
	void CheckCombatTarget();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

protected:
	UPROPERTY(VIsibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(VIsibleAnywhere, Category = Montages)
	UAnimMontage* DeathMontage;

	UPROPERTY()
	AAIController* MonsterController;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* CurrentPatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 300.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;

	UPROPERTY(BlueprintReadOnly)
	EMonsterState MonsterState = EMonsterState::EMS_Patrolling;

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

	FTimerHandle PatrolTimer;
};
