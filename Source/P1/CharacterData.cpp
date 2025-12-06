// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterData.h"
#include "CharacterDataTableRow.h"
#include "Engine/DataTable.h"

#if WITH_EDITOR
void UCharacterData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// DataTableRowName이 변경되었을 때
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UCharacterData, DataTableRowName))
	{
		if (!DataTableRowName.IsNone())
		{
			// SourceDataTable 사용 또는 기본 경로
			UDataTable* DataTable = SourceDataTable.LoadSynchronous();
			
			if (!DataTable)
			{
				// 기본 경로에서 로드 시도
				FString TablePath = TEXT("/Game/Data/CharacterDataTable.CharacterDataTable");
				DataTable = LoadObject<UDataTable>(nullptr, *TablePath);
			}

			if (DataTable)
			{
				// Row 찾기
				FCharacterDataTableRow* RowData = DataTable->FindRow<FCharacterDataTableRow>(DataTableRowName, TEXT(""));
				
				if (RowData)
				{
					// 스탯 자동 임포트
					CharacterName = RowData->CharacterName;
					Rarity = RowData->GetRarityEnum();
					BaseStats.MaxHealth = RowData->MaxHealth;
					BaseStats.AttackPower = RowData->AttackPower;
					BaseStats.Defense = RowData->Defense;
					BaseStats.AttackSpeed = RowData->AttackSpeed;
					BaseStats.MoveSpeed = RowData->MoveSpeed;
					Description = FText::FromString(RowData->Description);

					UE_LOG(LogTemp, Log, TEXT("Imported stats from DataTable row: %s"), *DataTableRowName.ToString());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Row '%s' not found in DataTable"), *DataTableRowName.ToString());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("CharacterDataTable not found"));
			}
		}
	}
}

TArray<FString> UCharacterData::GetDataTableRowNames() const
{
	TArray<FString> RowNames;

	// SourceDataTable 사용 또는 기본 경로
	UDataTable* DataTable = SourceDataTable.LoadSynchronous();
	
	if (!DataTable)
	{
		// 기본 경로에서 로드 시도
		FString TablePath = TEXT("/Game/Data/CharacterDataTable.CharacterDataTable");
		DataTable = LoadObject<UDataTable>(nullptr, *TablePath);
	}

	if (DataTable)
	{
		TArray<FName> AllRowNames = DataTable->GetRowNames();
		for (const FName& RowName : AllRowNames)
		{
			RowNames.Add(RowName.ToString());
		}
	}

	return RowNames;
}
#endif

