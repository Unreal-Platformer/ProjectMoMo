// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MainMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenu::StartNewGame()
{
	UGameplayStatics::OpenLevel(this, FName("ThirdPersonMap"));
}

void UMainMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, 0, EQuitPreference::Quit, false);
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(NewGameButton)
		NewGameButton->OnClicked.AddDynamic(this, &UMainMenu::StartNewGame);

	if(QuitButton)
		QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
}
