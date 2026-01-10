// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "StageData.generated.h"

/**
 * 스테이지 DataTable Row (CSV 임포트용)
 */
USTRUCT(BlueprintType)
struct FStageDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StageName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StageLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MapPositionX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MapPositionY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GoldReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ExpReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUnlocked;
};

/**
 * 스테이지 정보 DataAsset
 * - CSV에서 데이터 자동 임포트
 */
UCLASS(BlueprintType)
class P1_API UStageData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// ========== DataTable 임포트 ==========
	
	UPROPERTY(EditAnywhere, Category = "Data Import")
	TObjectPtr<UDataTable> DataTable;

	UPROPERTY(EditAnywhere, Category = "Data Import", meta = (GetOptions = "GetDataTableRowNames"))
	FName DataTableRowName;

	UFUNCTION()
	TArray<FString> GetDataTableRowNames() const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// ========== 스테이지 기본 정보 ==========
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage Info")
	FString StageName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage Info")
	int32 StageLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage Info")
	FText Description;

	// ========== 미니맵 표시 ==========
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	FVector2D MapPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TObjectPtr<UTexture2D> StageIcon;

	// ========== 전투 설정 ==========
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle")
	TArray<TObjectPtr<class UCharacterData>> EnemyCharacters;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle")
	int32 EnemyCount = 3;

	// ========== 보상 ==========
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rewards")
	int32 GoldReward = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rewards")
	int32 ExpReward = 50;

	// ========== 해금 조건 ==========
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlock")
	bool bIsUnlocked = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlock")
	TObjectPtr<UStageData> RequiredPreviousStage;
};
