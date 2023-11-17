// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/CharacterStatComponent.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	// ...
}


// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterStatComponent::SetDamage(float NewDamage)
{
	SetLifePoint(FMath::Clamp<float>(CurrentLifePoint - NewDamage, 0.0f, MaxLifePoint));
}

void UCharacterStatComponent::SetLifePoint(float NewLifePoint)
{
	CurrentLifePoint = NewLifePoint;
	OnLifePointChanged.Broadcast();
	if (CurrentLifePoint < KINDA_SMALL_NUMBER)
	{
		CurrentLifePoint = 0.0f;
		OnLifePointIsZero.Broadcast();
	}
}

void UCharacterStatComponent::UseTimePoint(float UsePoint)
{
	SetLifePoint(FMath::Clamp<float>(CurrentLifePoint - UsePoint, 0.0f, MaxLifePoint));
}

void UCharacterStatComponent::SetTimePoint(float NewTimePoint)
{
	CurrentTimePoint = NewTimePoint;
	OnTimePointChanged.Broadcast();
	if (CurrentTimePoint < KINDA_SMALL_NUMBER)
		CurrentTimePoint = 0.0f;
}

