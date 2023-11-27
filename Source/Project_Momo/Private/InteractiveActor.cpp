// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActor.h"

#include "Project_Momo/Project_MomoCharacter.h"

// Sets default values
AInteractiveActor::AInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentAppliedSkill = EAppliedSkill::None;
}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	InitActorPosition = GetActorLocation();
	ActorStaticMeshComponent = GetComponentByClass<UStaticMeshComponent>();
	ActorStaticMeshComponent->OnComponentHit.AddDynamic(this, &AInteractiveActor::HandleCollision);
	PositionHistory.Reserve(2000);
}

// Called every frame
void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentAppliedSkill != EAppliedSkill::Rewind)
	{
		PositionHistory.Push(ActorStaticMeshComponent->GetComponentLocation());
		RotationHistory.Push(ActorStaticMeshComponent->GetComponentRotation());
		if(PositionHistory.Num() > 1000 || RotationHistory.Num() > 1000)
		{
			PositionHistory.RemoveAt(0);
			RotationHistory.RemoveAt(0);
		}
	}
	else
	{
		ActorStaticMeshComponent->SetWorldLocation(PositionHistory.Last());
		ActorStaticMeshComponent->SetWorldRotation(RotationHistory.Last());
		PositionHistory.Pop();
		RotationHistory.Pop();
		if(PositionHistory.Num() == 0 || RotationHistory.Num() == 0)
		{
			CurrentAppliedSkill = EAppliedSkill::None;
			CurrentActorLinearVelocity = ActorStaticMeshComponent->GetPhysicsLinearVelocity();
			CurrentActorAngularVelocity = ActorStaticMeshComponent->GetPhysicsAngularVelocityInRadians();
			ActorStaticMeshComponent->SetSimulatePhysics(true);
			ActorStaticMeshComponent->SetPhysicsLinearVelocity(-CurrentActorLinearVelocity);
			ActorStaticMeshComponent->SetPhysicsAngularVelocityInRadians(-CurrentActorAngularVelocity);
		}
	}
}

void AInteractiveActor::ResetActorPosition()
{
	ActorStaticMeshComponent->SetPhysicsLinearVelocity(FVector(0, 0, 0));
	ActorStaticMeshComponent->SetPhysicsAngularVelocityInRadians(FVector(0, 0, 0));
	SetActorLocation(InitActorPosition);
}

void AInteractiveActor::ReversingActorPosition()
{
	ActorStaticMeshComponent->SetSimulatePhysics(false);
	for(TCheckedPointerIterator<FVector, int, true> Iter = PositionHistory.rbegin(); Iter != PositionHistory.rend(); ++Iter)
	{
		ActorStaticMeshComponent->SetWorldLocation(*Iter);
		if(CurrentAppliedSkill != EAppliedSkill::Rewind)
			break;
	}
	CurrentAppliedSkill = EAppliedSkill::None;
	ActorStaticMeshComponent->SetSimulatePhysics(true);
}

void AInteractiveActor::ApplySkill(EAppliedSkill ApplySkillType)
{
	if (CurrentAppliedSkill != EAppliedSkill::None)
		return;
	
	CurrentActorLinearVelocity = ActorStaticMeshComponent->GetPhysicsLinearVelocity();
	CurrentActorAngularVelocity = ActorStaticMeshComponent->GetPhysicsAngularVelocityInRadians();
	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor(255,255,0), CurrentActorLinearVelocity.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor(255,255,0), CurrentActorAngularVelocity.ToString());
	
	CurrentAppliedSkill = ApplySkillType;
	switch (ApplySkillType)
	{
		case EAppliedSkill::Rewind:
			ActorStaticMeshComponent->SetSimulatePhysics(false);
			break;
		case EAppliedSkill::Quicken:
			ActorStaticMeshComponent->SetPhysicsLinearVelocity(2 * CurrentActorLinearVelocity);
			ActorStaticMeshComponent->SetPhysicsAngularVelocityInRadians(2 * CurrentActorAngularVelocity);
			break;
		case EAppliedSkill::Slow:
			ActorStaticMeshComponent->SetPhysicsLinearVelocity(0.5f * CurrentActorLinearVelocity);
			ActorStaticMeshComponent->SetPhysicsAngularVelocityInRadians(0.5f * CurrentActorAngularVelocity);
			break;
		case EAppliedSkill::Stop:
			ActorStaticMeshComponent->SetSimulatePhysics(false);
			ActorStaticMeshComponent->SetPhysicsLinearVelocity(FVector(0, 0, 0));
			ActorStaticMeshComponent->SetPhysicsAngularVelocityInRadians(FVector(0, 0, 0));
			break;
		default:
			UE_LOG(LogTemp, Fatal, TEXT("%s: Incorrect ApplySkillType."), *GetName());
			return;
	}
}

void AInteractiveActor::CancelAppliedSkill()
{
	if(CurrentAppliedSkill != EAppliedSkill::Stop)
	{
		CurrentActorLinearVelocity = ActorStaticMeshComponent->GetPhysicsLinearVelocity();
		CurrentActorAngularVelocity = ActorStaticMeshComponent->GetPhysicsAngularVelocityInRadians();
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor(255,255,255), CurrentActorLinearVelocity.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor(255,255,255), CurrentActorAngularVelocity.ToString());

	switch (CurrentAppliedSkill)
	{
		case EAppliedSkill::Rewind:
			ActorStaticMeshComponent->SetSimulatePhysics(true);
			ActorStaticMeshComponent->SetPhysicsLinearVelocity(-CurrentActorLinearVelocity);
			ActorStaticMeshComponent->SetPhysicsAngularVelocityInRadians(-CurrentActorAngularVelocity);
			break;
		case EAppliedSkill::Quicken:
			ActorStaticMeshComponent->SetPhysicsLinearVelocity(0.5f * CurrentActorLinearVelocity);
			ActorStaticMeshComponent->SetPhysicsAngularVelocityInRadians(0.5f * CurrentActorAngularVelocity);
			break;
		case EAppliedSkill::Slow:
			ActorStaticMeshComponent->SetPhysicsLinearVelocity(2 * CurrentActorLinearVelocity);
			ActorStaticMeshComponent->SetPhysicsAngularVelocityInRadians(2 * CurrentActorAngularVelocity);
			break;
	case EAppliedSkill::Stop:
			ActorStaticMeshComponent->SetSimulatePhysics(true);
			ActorStaticMeshComponent->SetPhysicsLinearVelocity(CurrentActorLinearVelocity);
			ActorStaticMeshComponent->SetPhysicsAngularVelocityInRadians(CurrentActorAngularVelocity);
			break;
		default:
			return;
	}
	CurrentAppliedSkill = EAppliedSkill::None;
}

void AInteractiveActor::HandleCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if((OtherActor->IsA(AInteractiveActor::StaticClass()) || OtherActor->IsA(AProject_MomoCharacter::StaticClass()))
		&& CurrentAppliedSkill == EAppliedSkill::Rewind)
		CancelAppliedSkill();
}

