// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CrosshairWidget.h"
#include "Components/Image.h"

void UCrosshairWidget::SetPicking(bool Picking)
{
	FLinearColor ImageColor(1.f, 1.f, 1.f, 1.f);
	if (Picking)
	{
		ImageColor.B = 0.f;
		ImageColor.G = 0.8f;
	}

	DotImage->SetColorAndOpacity(ImageColor);
}

void UCrosshairWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
