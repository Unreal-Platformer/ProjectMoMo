// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/UMGPlayerController.h"

void AUMGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameAndUI());
}
