// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DefaultPlayerController.generated.h"

class UHUDView;
class UCrosshairWidget;

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
	inline UCrosshairWidget* GetCrosshairWidget() const { return CrosshairWidget; };

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<UUserWidget> HUDViewClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<UUserWidget> CrosshairClass;

private:
	UPROPERTY()
	UHUDView* HUDView;

	UPROPERTY()
	UCrosshairWidget* CrosshairWidget;
};
