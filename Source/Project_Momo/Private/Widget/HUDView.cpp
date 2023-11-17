// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HUDView.h"
#include "Components/ProgressBar.h"
#include "ActorComponent/CharacterStatComponent.h"

void UHUDView::BindCharacterStat(UCharacterStatComponent* NewCharacterStat)
{
	if (nullptr == NewCharacterStat)
		return;

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnLifePointChanged.AddUObject(this, &UHUDView::UpdateCharacterLifePoint);
	NewCharacterStat->OnTimePointChanged.AddUObject(this, &UHUDView::UpdateCharacterTimePoint);
}

void UHUDView::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHUDView::UpdateCharacterLifePoint()
{
	if (CurrentCharacterStat->IsValidLowLevel())
	{
		if (nullptr != LifePointProgressBar)
			LifePointProgressBar->SetPercent(CurrentCharacterStat->GetLifePointRatio());
	}
}

void UHUDView::UpdateCharacterTimePoint()
{
	if (CurrentCharacterStat->IsValidLowLevel())
	{
		if (nullptr != TimePointProgressBar)
			TimePointProgressBar->SetPercent(CurrentCharacterStat->GetTimePointRatio());
	}
}

