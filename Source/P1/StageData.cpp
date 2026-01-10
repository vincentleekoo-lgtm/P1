// Fill out your copyright notice in the Description page of Project Settings.

#include "StageData.h"

#if WITH_EDITOR
#include "Engine/DataTable.h"

TArray<FString> UStageData::GetDataTableRowNames() const
{
	TArray<FString> RowNames;
	
	if (DataTable)
	{
		TArray<FName> RowNamesFName = DataTable->GetRowNames();
		for (const FName& RowName : RowNamesFName)
		{
			RowNames.Add(RowName.ToString());
		}
	}
	
	return RowNames;
}

void UStageData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.GetPropertyName();

	// DataTableRowName이 변경되면 데이터 자동 임포트
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UStageData, DataTableRowName))
	{
		if (DataTable && !DataTableRowName.IsNone())
		{
			FStageDataTableRow* Row = DataTable->FindRow<FStageDataTableRow>(DataTableRowName, TEXT(""));
			if (Row)
			{
				StageName = Row->StageName;
				StageLevel = Row->StageLevel;
				Description = FText::FromString(Row->Description);
				MapPosition = FVector2D(Row->MapPositionX, Row->MapPositionY);
				GoldReward = Row->GoldReward;
				ExpReward = Row->ExpReward;
				EnemyCount = Row->EnemyCount;
				bIsUnlocked = Row->bIsUnlocked;

				UE_LOG(LogTemp, Log, TEXT("StageData imported from DataTable: %s (MapPos: %.1f, %.1f)"), 
					*StageName, MapPosition.X, MapPosition.Y);
			}
		}
	}
}
#endif
