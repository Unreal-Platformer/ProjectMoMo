// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDView.generated.h"

class UProgressBar;
class UCharacterStatComponent;

/**
 * 
 */
UCLASS()
class PROJECT_MOMO_API UHUDView : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(UCharacterStatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterLifePoint();
	void UpdateCharacterTimePoint();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* LifePointProgressBar;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* TimePointProgressBar;

private:
	TWeakObjectPtr<class UCharacterStatComponent> CurrentCharacterStat;
};
