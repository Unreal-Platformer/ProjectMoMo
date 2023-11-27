// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MainMenuGameMode.h"
#include "Widget/MainMenu.h"
#include "Blueprint/UserWidget.h"


AMainMenuGameMode::AMainMenuGameMode()
{
}

void AMainMenuGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidget)
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidget && GetWorld())
	{
		CurrentWidget = CreateWidget(GetWorld(), NewWidget);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartWidget);
}