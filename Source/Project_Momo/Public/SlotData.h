// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SlotData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MOMO_API USlotData : public USaveGame
{
	GENERATED_BODY()

public:
	USlotData();

public:
	UPROPERTY()
	FVector PlayerPos;
};
