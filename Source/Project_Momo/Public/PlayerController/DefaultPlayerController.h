// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DefaultPlayerController.generated.h"

class UHUDView;

/**
 * 
 */
UCLASS()
class PROJECT_MOMO_API ADefaultPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADefaultPlayerController();

	inline UHUDView* GetHUDView() const { return HUDView; };

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UHUDView> HUDViewClass;

private:
	UPROPERTY()
	UHUDView* HUDView;
};
