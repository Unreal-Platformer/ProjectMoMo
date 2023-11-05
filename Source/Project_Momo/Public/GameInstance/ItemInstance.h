// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ItemInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MOMO_API UItemInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UItemInstance();
	struct FAITemData* GetItemDataFromRow(int32 Row);


private:
	UPROPERTY()
	class UDataTable* AItemTable;
};
