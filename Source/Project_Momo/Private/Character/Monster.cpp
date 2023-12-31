// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "ActorComponent/CharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationData.h"
#include "AIController.h"

// Sets default values
AMonster::AMonster()
	: ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	MonsterController = Cast<AAIController>(GetController());
	//MoveToTarget(CurrentPatrolTarget);

	GetWorldTimerManager().SetTimer(PatrolTimer, this, &AMonster::PatrolTimerFinished, 5.f);
	GetCharacterMovement()->MaxWalkSpeed = 125.f;

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AMonster::PawnSeen);
	}
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MonsterState > EMonsterState::EMS_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}

}

void AMonster::CheckPatrolTarget()
{
	if (MonsterController && InTargetRange(CurrentPatrolTarget, PatrolRadius))
	{
		CurrentPatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AMonster::PatrolTimerFinished, WaitTime);
	}
}

void AMonster::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, CombatRadius))
	{
		CombatTarget = nullptr;
		MonsterState = EMonsterState::EMS_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		MoveToTarget(CurrentPatrolTarget);
	}
	else if(MonsterState != EMonsterState::EMS_Chasing && !InTargetRange(CombatTarget, AttackRadius))
	{
		MonsterState = EMonsterState::EMS_Chasing;
		MoveToTarget(CombatTarget);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
	else if (MonsterState != EMonsterState::EMS_Attacking && InTargetRange(CombatTarget, AttackRadius))
	{
		MonsterState = EMonsterState::EMS_Attacking;
	}
}

void AMonster::PawnSeen(APawn* SeenPawn)
{
	if (MonsterState == EMonsterState::EMS_Chasing)
		return;

	if (SeenPawn->ActorHasTag(FName("MomoCharacter")))
	{
		MonsterState = EMonsterState::EMS_Chasing;

		GetWorldTimerManager().ClearTimer(PatrolTimer);

		CombatTarget = SeenPawn;
		MoveToTarget(CombatTarget);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		const int32 Selection = FMath::RandRange(0, 3);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("DeathRight");
			DeathPose = EDeathPose::EDP_DeathRight;
			break;
		case 1:
			SectionName = FName("DeathLeft");
			DeathPose = EDeathPose::EDP_DeathLeft;
			break;
		case 2:
			SectionName = FName("DeathForward");
			DeathPose = EDeathPose::EDP_DeathFront;
			break;
		case 3:
			SectionName = FName("DeathBackward");
			DeathPose = EDeathPose::EDP_DeathBack;
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.f);
}

void AMonster::Attack()
{
}

void AMonster::AttackEnd()
{
}

bool AMonster::CanAttack()
{
	return false;
}

void AMonster::PlayAttackMontage()
{
}

bool AMonster::InTargetRange(AActor* Target, const double Radius) const
{
	if (Target == nullptr)
		return false;

	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return Radius >= DistanceToTarget;
}

void AMonster::MoveToTarget(AActor* Target)
{
	if (MonsterController == nullptr || Target == nullptr)
		return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);
	MonsterController->MoveTo(MoveRequest);
}

AActor* AMonster::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != CurrentPatrolTarget)
			ValidTargets.AddUnique(Target);
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (0 < NumPatrolTargets)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}

	return nullptr;
}

void AMonster::PatrolTimerFinished()
{
	MoveToTarget(CurrentPatrolTarget);
}

