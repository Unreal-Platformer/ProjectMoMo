// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActor.h"

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
	
}

// Called every frame
void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractiveActor::ResetActorPosition()
{
	UStaticMeshComponent* ActorStaticMeshComponent = GetComponentByClass<UStaticMeshComponent>();
	ActorStaticMeshComponent->SetPhysicsLinearVelocity(FVector(0, 0, 0));
	ActorStaticMeshComponent->SetPhysicsAngularVelocityInRadians(FVector(0, 0, 0));
	SetActorLocation(InitActorPosition);
}

void AInteractiveActor::ApplySkill(EAppliedSkill ApplySkillType)
{
	if (CurrentAppliedSkill != EAppliedSkill::None)
		return;
	
	UStaticMeshComponent* ActorStaticMeshComponent = GetComponentByClass<UStaticMeshComponent>();
	CurrentActorLinearVelocity = ActorStaticMeshComponent->GetPhysicsLinearVelocity();
	CurrentActorAngularVelocity = ActorStaticMeshComponent->GetPhysicsAngularVelocityInRadians();
	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor(255,255,0), CurrentActorLinearVelocity.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor(255,255,0), CurrentActorAngularVelocity.ToString());
	
	switch (ApplySkillType)
	{
		case EAppliedSkill::Rewind:
			ActorStaticMeshComponent->SetPhysicsLinearVelocity(-CurrentActorLinearVelocity);
			ActorStaticMeshComponent->SetPhysicsAngularVelocityInRadians(-CurrentActorAngularVelocity);
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
	CurrentAppliedSkill = ApplySkillType;
}

void AInteractiveActor::CancelAppliedSkill()
{
	UStaticMeshComponent* ActorStaticMeshComponent = GetComponentByClass<UStaticMeshComponent>();
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

void AInteractiveActor::HandleCollision()
{
	UE_LOG(LogTemp, )
}

