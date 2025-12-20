// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageManager.generated.h"

class ASpawnManager;
class ABattleManager;

UENUM(BlueprintType)
enum class EStageState : uint8
{
	NotStarted UMETA(DisplayName = "NotStarted"),
	Preparing UMETA(DisplayName = "Preparing"),
	InProgress UMETA(DisplayName = "InProgress"),
	Completed UMETA(DisplayName = "Completed"),
	Failed UMETA(DisplayName = "Failed")
};

/**
 * 스테이지 관리자
 * - 현재 스테이지 정보 관리
 * - 웨이브/목표 관리
 * - SpawnManager와 BattleManager 소유
 */
UCLASS()
class P1_API AStageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AStageManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 스테이지 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	int32 StageNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FString StageName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
	EStageState CurrentStageState;

	// 자동 시작 옵션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	bool bAutoStartOnBeginPlay;

	// 스폰 매니저 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Managers")
	ASpawnManager* SpawnManager;

	// 배틀 매니저 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Managers")
	ABattleManager* BattleManager;

	// ========== 스테이지 초기화 (신규) ==========

	// StageData 기반 초기화
	UFUNCTION(BlueprintCallable, Category = "Stage")
	void InitializeWithStageData(class UStageData* InStageData, const TArray<struct FBattleSlot>& PlayerFormation);

	// 스테이지 데이터
	UPROPERTY(BlueprintReadOnly, Category = "Stage")
	TObjectPtr<class UStageData> StageData;

	// 플레이어 편성
	UPROPERTY(BlueprintReadOnly, Category = "Stage")
	TArray<struct FBattleSlot> PlayerFormation;

	// ========== 기존 함수들 ==========

	// 스테이지 시작
	UFUNCTION(BlueprintCallable, Category = "Stage")
	void StartStage();

	// 스테이지 완료
	UFUNCTION(BlueprintCallable, Category = "Stage")
	void CompleteStage();

	// 스테이지 실패
	UFUNCTION(BlueprintCallable, Category = "Stage")
	void FailStage();

	// 웨이브 관리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 TotalWaves;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWave;

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void StartNextWave();

	// BattleManager 이벤트 핸들러
	UFUNCTION()
	void HandleBattleCompleted(bool bVictory);

private:
	void ChangeStageState(EStageState NewState);
};
