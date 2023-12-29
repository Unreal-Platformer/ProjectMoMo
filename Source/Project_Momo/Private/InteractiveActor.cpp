// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActor.h"

#include "Project_Momo/Project_MomoCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteractiveActor::AInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentAppliedSkill = EAppliedSkill::None;
	
	static ConstructorHelpers::FObjectFinder<UMaterial> FresnelMaterialFinder(TEXT("/Script/Engine.Material'/Game/Material/FresnelMaterial.FresnelMaterial'"));
	if (FresnelMaterialFinder.Succeeded())
	{
		FresnelMaterialBase = FresnelMaterialFinder.Object;
		if (FresnelMaterialBase.IsValid())
		{
			FresnelMaterialInstance = CreateDefaultSubobject<UMaterialInstanceDynamic>(FName("Fresnel Material Instance"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Material Load Failed"), *this->GetName());
	}
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemFinder(TEXT("/Script/Niagara.NiagaraSystem'/Game/Particles/SkillApplyEffect.SkillApplyEffect'"));
	if (NiagaraSystemFinder.Succeeded())
	{
		NiagaraSystemBase = NiagaraSystemFinder.Object.Get();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Niagara System Load Failed"), *this->GetName());
	}

	
}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	InitActorPosition = GetActorLocation();
	
	ActorStaticMeshComponent = GetComponentByClass<UStaticMeshComponent>();
	ActorStaticMeshComponent->OnComponentHit.AddDynamic(this, &AInteractiveActor::HandleCollision);

	ActorNiagaraSystemComponent = GetComponentByClass<UNiagaraComponent>();

	if (ActorNiagaraSystemComponent != nullptr)
	{
		ActorNiagaraSystemComponent->SetAsset(NiagaraSystemBase.Get());
		ActorNiagaraSystemComponent->Deactivate();
	}
	
	PositionHistory.Reserve(10000);
	RotationHistory.Reserve(10000);
	
	if (FresnelMaterialBase.IsValid())
	{
		FresnelMaterialInstance->Parent = FresnelMaterialBase.Get();
		FresnelMaterialInstance->CopyMaterialUniformParameters(FresnelMaterialBase.Get());
		FresnelMaterialInstance->SetVectorParameterValue(TEXT("Fresnel Color"), FLinearColor(0,0,0,0));
		ActorStaticMeshComponent->SetOverlayMaterial(FresnelMaterialInstance);
	}

	PlayerCharacter = Cast<AProject_MomoCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerCharacter->ReadySkillEvent.AddUObject(this, &AInteractiveActor::ChangeEffectColorForReadySkill);
}

// Called every frame
void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentAppliedSkill != EAppliedSkill::Rewind)
	{
		const FVector& CurrLocation = ActorStaticMeshComponent->GetComponentLocation();
		const FRotator& CurrRotation = ActorStaticMeshComponent->GetComponentRotation();
		if(PlayerCharacter->ReadySkillState == EReadySkillState::Unready && PositionHistory.Num() != 0)
		{
			for(int32 loop = 1; loop <= 5; loop++)
			{
				PositionHistory.Push(((CurrLocation - PositionHistory.Last()) * loop * 0.2) + PositionHistory.Last());
				RotationHistory.Push(((CurrRotation - RotationHistory.Last()) * loop * 0.2) + RotationHistory.Last());
			}
		}
		else
		{
			PositionHistory.Push(CurrLocation);
			RotationHistory.Push(CurrRotation);
		}
		if(PositionHistory.Num() > 5000 || RotationHistory.Num() > 5000)
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
		if(PlayerCharacter->ReadySkillState == EReadySkillState::Unready)
		{
			for(int32 loop = 0; loop < 4; loop++)
			{
				if(PositionHistory.Num() == 0 || RotationHistory.Num() == 0)
					break;
				PositionHistory.Pop();
				RotationHistory.Pop();
			}
		}
		if(PositionHistory.Num() == 0 || RotationHistory.Num() == 0)
		{
			CancelAppliedSkill();
		}
	}
}

void AInteractiveActor::ChangeEffectColor(const EAppliedSkill& ApplySkillType)
{
	switch (ApplySkillType)
	{
	case EAppliedSkill::Rewind:
		FresnelMaterialInstance->SetVectorParameterValue(TEXT("Fresnel Color"), FLinearColor(0.5,4.2,0.4, 1));
		ActorNiagaraSystemComponent->SetVariableVec3(FName("Scale RGB"), FVector(0.5,4.2,0.4));
		break;
	case EAppliedSkill::Quicken:
		FresnelMaterialInstance->SetVectorParameterValue(TEXT("Fresnel Color"), FLinearColor(8.7, 1.1, 1.1, 1));
		ActorNiagaraSystemComponent->SetVariableVec3(FName("Scale RGB"), FVector(8.7,1.1,1.1));
		break;
	case EAppliedSkill::Slow:
		FresnelMaterialInstance->SetVectorParameterValue(TEXT("Fresnel Color"), FLinearColor(9.3, 8.2, 1.55,1));
		ActorNiagaraSystemComponent->SetVariableVec3(FName("Scale RGB"), FVector(9.3,8.2,1.55));
		break;
	case EAppliedSkill::Stop:
		FresnelMaterialInstance->SetVectorParameterValue(TEXT("Fresnel Color"), FLinearColor(1.91, 4.0, 10.0,1));
		ActorNiagaraSystemComponent->SetVariableVec3(FName("Scale RGB"), FVector(1.91,4.0,10.0));
		break;
	case EAppliedSkill::None:
		if (PlayerCharacter->ReadySkillState == EReadySkillState::Ready)
			FresnelMaterialInstance->SetVectorParameterValue(TEXT("Fresnel Color"), FLinearColor(10,0,0, 1));
		else
			FresnelMaterialInstance->SetVectorParameterValue(TEXT("Fresnel Color"), FLinearColor(0,0,0, 0));
	}
	
	ActorStaticMeshComponent->SetOverlayMaterial(FresnelMaterialInstance);
	if (ApplySkillType == EAppliedSkill::None)
		ActorNiagaraSystemComponent->Deactivate();
	else
		ActorNiagaraSystemComponent->Activate();
}

void AInteractiveActor::ChangeEffectColorForReadySkill(EReadySkillState ReadySkillState)
{
	if(CurrentAppliedSkill != EAppliedSkill::None)
		return;
	
	if (PlayerCharacter->ReadySkillState == EReadySkillState::Ready)
		FresnelMaterialInstance->SetVectorParameterValue(TEXT("Fresnel Color"), FLinearColor(10,0,0, 1));
	else
		FresnelMaterialInstance->SetVectorParameterValue(TEXT("Fresnel Color"),  FLinearColor(0,0,0, 0));
	ActorStaticMeshComponent->SetOverlayMaterial(FresnelMaterialInstance);
}


void AInteractiveActor::ResetActorPosition()
{
	ActorStaticMeshComponent->SetPhysicsLinearVelocity(FVector(0, 0, 0));
	ActorStaticMeshComponent->SetPhysicsAngularVelocityInRadians(FVector(0, 0, 0));
	SetActorLocation(InitActorPosition);
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
	ChangeEffectColor(ApplySkillType);
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
	ChangeEffectColor(EAppliedSkill::None);
}

void AInteractiveActor::HandleCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if((OtherActor->IsA(AInteractiveActor::StaticClass()) || OtherActor->IsA(AProject_MomoCharacter::StaticClass()))
		&& CurrentAppliedSkill == EAppliedSkill::Rewind)
		CancelAppliedSkill();
}

