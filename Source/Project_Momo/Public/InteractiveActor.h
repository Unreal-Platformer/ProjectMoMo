// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"


enum class EReadySkillState : uint8;
class AProject_MomoCharacter;
class UNiagaraComponent;
class UNiagaraSystem;

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

	void ChangeEffectColor(const EAppliedSkill& AppliedSkill);
	void ChangeEffectColorForReadySkill(EReadySkillState ReadySkillState);

	UFUNCTION()
	void ResetActorPosition();

	UFUNCTION()
	void ApplySkill(EAppliedSkill ApplySkillType);
	
	UFUNCTION()
	void CancelAppliedSkill();

	UFUNCTION()
	void HandleCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY()
	UStaticMeshComponent* ActorStaticMeshComponent;

	UPROPERTY()
	UNiagaraComponent* ActorNiagaraSystemComponent;

	TWeakObjectPtr<AProject_MomoCharacter> PlayerCharacter;

	TWeakObjectPtr<UMaterialInterface> FresnelMaterialBase;
	
	TWeakObjectPtr<UNiagaraSystem> NiagaraSystemBase;
	
	UPROPERTY()
	UMaterialInstanceDynamic* FresnelMaterialInstance;

	UPROPERTY()
	FVector CurrentActorLinearVelocity;

	UPROPERTY()
	FVector CurrentActorAngularVelocity;
	
	UPROPERTY()
	FVector InitActorPosition;

	UPROPERTY()
	EAppliedSkill CurrentAppliedSkill;

	UPROPERTY()
	TArray<FVector> PositionHistory;

	UPROPERTY()
	TArray<FRotator> RotationHistory;
};
