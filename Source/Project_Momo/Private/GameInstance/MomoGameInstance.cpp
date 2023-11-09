// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MomoGameInstance.h"
#include "Item.h"

UMomoGameInstance::UMomoGameInstance()
{
	FString ItemDataPath = TEXT("DataTable'/Game/Items/Items.Items'"); 
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ABCHARACTER(*ItemDataPath);
	if (DT_ABCHARACTER.Succeeded())
	{
		AItemTable = DT_ABCHARACTER.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("���� �ε忡 �����߽��ϴ�. ��θ� Ȯ�����ּ���. '%s'"), *ItemDataPath);
	}
}

FAITemData* UMomoGameInstance::GetItemDataFromRow(int32 Row)
{
	if (!AItemTable)
		return nullptr;

	return AItemTable->FindRow<FAITemData>(*FString::FromInt(Row), TEXT(""));
}
