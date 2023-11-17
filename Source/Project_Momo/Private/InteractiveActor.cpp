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
	UPrimitiveComponent* ActorRootComponent = GetComponentByClass<UPrimitiveComponent>();
	ActorRootComponent->SetPhysicsLinearVelocity(FVector(0, 0, 0));
	ActorRootComponent->SetPhysicsAngularVelocityInRadians(FVector(0, 0, 0));
	SetActorLocation(InitActorPosition);
}

void AInteractiveActor::ApplySkill(EAppliedSkill ApplySkillType)
{
	if (CurrentAppliedSkill != EAppliedSkill::None)
		return;
	
	UPrimitiveComponent* ActorRootComponent = GetComponentByClass<UPrimitiveComponent>();
	CurrentActorLinearVelocity = ActorRootComponent->GetPhysicsLinearVelocity();
	CurrentActorAngularVelocity = ActorRootComponent->GetPhysicsAngularVelocityInRadians();
	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor(255,255,0), CurrentActorLinearVelocity.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor(255,255,0), CurrentActorAngularVelocity.ToString());
	
	switch (ApplySkillType)
	{
		case EAppliedSkill::Rewind:
			ActorRootComponent->SetPhysicsLinearVelocity(-CurrentActorLinearVelocity);
			ActorRootComponent->SetPhysicsAngularVelocityInRadians(-CurrentActorAngularVelocity);
			break;
		case EAppliedSkill::Quicken:
			ActorRootComponent->SetPhysicsLinearVelocity(2 * CurrentActorLinearVelocity);
			ActorRootComponent->SetPhysicsAngularVelocityInRadians(2 * CurrentActorAngularVelocity);
			break;
		case EAppliedSkill::Slow:
			ActorRootComponent->SetPhysicsLinearVelocity(0.5f * CurrentActorLinearVelocity);
			ActorRootComponent->SetPhysicsAngularVelocityInRadians(0.5f * CurrentActorAngularVelocity);
			break;
		case EAppliedSkill::Stop:
			ActorRootComponent->SetSimulatePhysics(false);
			ActorRootComponent->SetPhysicsLinearVelocity(FVector(0, 0, 0));
			ActorRootComponent->SetPhysicsAngularVelocityInRadians(FVector(0, 0, 0));
			break;
		default:
			UE_LOG(LogTemp, Fatal, TEXT("%s: Incorrect ApplySkillType."), *GetName());
			return;
	}
	CurrentAppliedSkill = ApplySkillType;
}

void AInteractiveActor::CancelAppliedSkill()
{
	UPrimitiveComponent* ActorRootComponent = GetComponentByClass<UPrimitiveComponent>();
	if(CurrentAppliedSkill != EAppliedSkill::Stop)
	{
		CurrentActorLinearVelocity = ActorRootComponent->GetPhysicsLinearVelocity();
		CurrentActorAngularVelocity = ActorRootComponent->GetPhysicsAngularVelocityInRadians();
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor(255,255,255), CurrentActorLinearVelocity.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor(255,255,255), CurrentActorAngularVelocity.ToString());

	switch (CurrentAppliedSkill)
	{
		case EAppliedSkill::Rewind:
			ActorRootComponent->SetPhysicsLinearVelocity(-CurrentActorLinearVelocity);
			ActorRootComponent->SetPhysicsAngularVelocityInRadians(-CurrentActorAngularVelocity);
			break;
		case EAppliedSkill::Quicken:
			ActorRootComponent->SetPhysicsLinearVelocity(0.5f * CurrentActorLinearVelocity);
			ActorRootComponent->SetPhysicsAngularVelocityInRadians(0.5f * CurrentActorAngularVelocity);
			break;
		case EAppliedSkill::Slow:
			ActorRootComponent->SetPhysicsLinearVelocity(2 * CurrentActorLinearVelocity);
			ActorRootComponent->SetPhysicsAngularVelocityInRadians(2 * CurrentActorAngularVelocity);
			break;
	case EAppliedSkill::Stop:
			ActorRootComponent->SetSimulatePhysics(true);
			ActorRootComponent->SetPhysicsLinearVelocity(CurrentActorLinearVelocity);
			ActorRootComponent->SetPhysicsAngularVelocityInRadians(CurrentActorAngularVelocity);
			break;
		default:
			return;
	}
	CurrentAppliedSkill = EAppliedSkill::None;
}

