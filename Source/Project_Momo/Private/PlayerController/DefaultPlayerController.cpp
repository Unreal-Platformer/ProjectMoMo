// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/DefaultPlayerController.h"
#include "Widget/HUDView.h"

ADefaultPlayerController::ADefaultPlayerController()
{
	static ConstructorHelpers::FClassFinder<UHUDView> UI_HUD_C(TEXT("/Game/Widget/BP_HUDView.BP_HUDView"));
	if (UI_HUD_C.Succeeded())
		HUDViewClass = UI_HUD_C.Class;
}

void ADefaultPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	HUDView = CreateWidget<UHUDView>(this, HUDViewClass);
	HUDView->AddToViewport();
}
