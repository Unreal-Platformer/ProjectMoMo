// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HUDView.h"
#include "Components/ProgressBar.h"
#include "ActorComponent/CharacterStatComponent.h"

void UHUDView::BindCharacterStat(UCharacterStatComponent* NewCharacterStat)
{
	if (nullptr == NewCharacterStat)
		return;

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UHUDView::UpdateCharacterStat);
}

void UHUDView::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHUDView::UpdateCharacterStat()
{
	if (CurrentCharacterStat->IsValidLowLevel())
	{
		if (nullptr != HPProgressBar)
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
	}
}
