// Fill out your copyright notice in the Description page of Project Settings.


#include "returnPoint.h"

#include "InteractiveActor.h"
#include "Components/BoxComponent.h"


// Sets default values
AreturnPoint::AreturnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(GetRootComponent());
	CollisionBox->SetCollisionProfileName(TEXT("ReturnPoint"));
	CollisionBox->SetBoxExtent(FVector(1.f,1.f,1.f));

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AreturnPoint::ReturnPointBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AreturnPoint::ReturnPointEndOverlap);
}

// Called when the game starts or when spawned
void AreturnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AreturnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AreturnPoint::ReturnPointBeginOverlap(UPrimitiveComponent* OverlappenComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor(255,255,0), TEXT("BeginOverlap"));

	AInteractiveActor* InteractiveActor = Cast<AInteractiveActor, AActor>(OtherActor);
	if(IsValid(InteractiveActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor(255,255,0), InteractiveActor->initActorPos.ToString());
		InteractiveActor->ResetActorPosition();
	}
}

void AreturnPoint::ReturnPointEndOverlap(UPrimitiveComponent* OverlappenComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor(255,0,255), TEXT("EndOverlap"));
	
}