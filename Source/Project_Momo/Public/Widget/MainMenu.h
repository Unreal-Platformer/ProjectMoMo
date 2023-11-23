// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class PROJECT_MOMO_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:	
	UFUNCTION()
	void StartNewGame();
	
	UFUNCTION()
	void QuitGame();

protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* NewGameButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ResumeGameButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* SettingButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* QuitButton;
};
