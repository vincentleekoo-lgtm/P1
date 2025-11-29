// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameManager.generated.h"

class AStageManager;

UENUM(BlueprintType)
enum class EGameState : uint8
{
	Login UMETA(DisplayName = "Login"),
	OutGame UMETA(DisplayName = "OutGame"),
	Cutscene UMETA(DisplayName = "Cutscene"),
	InBattle UMETA(DisplayName = "InBattle"),
	BattleResult UMETA(DisplayName = "BattleResult"),
	GameOver UMETA(DisplayName = "GameOver")
};

// 게임 상태 변경 이벤트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameStateChanged, EGameState, OldState, EGameState, NewState);

/**
 * 게임 전체 상태 관리자 (GameInstance Subsystem)
 * - 로그인/아웃게임 관리
 * - 컷신 재생
 * - 스테이지 전환
 * - 게임 플로우 제어
 */
UCLASS()
class P1_API UGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Subsystem 초기화
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// 현재 게임 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
	EGameState CurrentGameState;

	// 게임 상태 변경 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Game State")
	FOnGameStateChanged OnGameStateChanged;

	// 게임 상태 전환
	UFUNCTION(BlueprintCallable, Category = "Game State")
	void ChangeGameState(EGameState NewState);

	// 스테이지 로드
	UFUNCTION(BlueprintCallable, Category = "Stage")
	void LoadStage(int32 StageNumber);

	// 컷신 재생
	UFUNCTION(BlueprintCallable, Category = "Cutscene")
	void PlayCutscene(const FString& CutsceneName);

	// 전투 시작
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void StartBattle();

	// 전투 종료
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void EndBattle(bool bVictory);

	// 게임 오버
	UFUNCTION(BlueprintCallable, Category = "Game")
	void GameOver();

	// 메인 메뉴로 돌아가기
	UFUNCTION(BlueprintCallable, Category = "Game")
	void ReturnToMainMenu();

private:
	// 현재 스테이지 번호
	UPROPERTY()
	int32 CurrentStageNumber;

	// 스테이지 매니저 참조
	UPROPERTY()
	AStageManager* CurrentStageManager;
};
