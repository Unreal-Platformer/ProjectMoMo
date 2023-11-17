// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

UENUM()
enum class EAppliedSkill : uint8
{
	None,
	Rewind,
	Quicken,
	Slow,
	Stop
};

UCLASS()
class PROJECT_MOMO_API AInteractiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ResetActorPosition();

	UFUNCTION()
	void ApplySkill(EAppliedSkill ApplySkillType);
	
	UFUNCTION()
	void CancelAppliedSkill();

	UFUNCTION()
	void ColideAction();
	
	UPROPERTY()
	FVector CurrentActorLinearVelocity;

	UPROPERTY()
	FVector CurrentActorAngularVelocity;
	
	UPROPERTY()
	FVector InitActorPosition;

	UPROPERTY()
	EAppliedSkill CurrentAppliedSkill;
};
