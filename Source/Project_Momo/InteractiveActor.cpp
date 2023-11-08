// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActor.h"

// Sets default values
AInteractiveActor::AInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	initActorPos = GetActorLocation();
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
	SetActorLocation(initActorPos);
}
