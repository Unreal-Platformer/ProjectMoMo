// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"

USTRUCT(BlueprintType)
struct FAITemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FAITemData() : ItemName(""), Details("") {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString Details;
};

UCLASS()
class PROJECT_MOMO_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
