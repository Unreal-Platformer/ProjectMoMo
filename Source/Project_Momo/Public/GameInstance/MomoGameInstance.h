// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MomoGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MOMO_API UMomoGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMomoGameInstance();
	struct FAITemData* GetItemDataFromRow(int32 Row);


private:
	UPROPERTY()
	class UDataTable* AItemTable;
};
