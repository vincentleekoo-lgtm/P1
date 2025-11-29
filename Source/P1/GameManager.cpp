// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"
#include "Kismet/GameplayStatics.h"

void UGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 초기 상태는 Login
	CurrentGameState = EGameState::Login;
	CurrentStageNumber = 0;
	CurrentStageManager = nullptr;

	UE_LOG(LogTemp, Log, TEXT("GameManager Initialized - State: Login"));
}

void UGameManager::Deinitialize()
{
	Super::Deinitialize();
	UE_LOG(LogTemp, Log, TEXT("GameManager Deinitialized"));
}

void UGameManager::ChangeGameState(EGameState NewState)
{
	if (CurrentGameState == NewState)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameState is already %d"), static_cast<int32>(NewState));
		return;
	}

	EGameState OldState = CurrentGameState;
	CurrentGameState = NewState;

	// 상태 변경 로그
	UE_LOG(LogTemp, Log, TEXT("GameState Changed: %d -> %d"), static_cast<int32>(OldState), static_cast<int32>(NewState));

	// 이벤트 브로드캐스트
	OnGameStateChanged.Broadcast(OldState, NewState);

	// 상태별 처리
	switch (NewState)
	{
	case EGameState::Login:
		UE_LOG(LogTemp, Log, TEXT("Entering Login State"));
		break;

	case EGameState::OutGame:
		UE_LOG(LogTemp, Log, TEXT("Entering OutGame State"));
		break;

	case EGameState::Cutscene:
		UE_LOG(LogTemp, Log, TEXT("Entering Cutscene State"));
		break;

	case EGameState::InBattle:
		UE_LOG(LogTemp, Log, TEXT("Entering InBattle State"));
		break;

	case EGameState::BattleResult:
		UE_LOG(LogTemp, Log, TEXT("Entering BattleResult State"));
		break;

	case EGameState::GameOver:
		UE_LOG(LogTemp, Log, TEXT("Entering GameOver State"));
		break;
	}
}

void UGameManager::LoadStage(int32 StageNumber)
{
	CurrentStageNumber = StageNumber;
	UE_LOG(LogTemp, Log, TEXT("Loading Stage: %d"), StageNumber);

	// TODO: 실제 레벨 로딩 구현
	// UGameplayStatics::OpenLevel(this, FName(*FString::Printf(TEXT("Stage_%d"), StageNumber)));

	ChangeGameState(EGameState::InBattle);
}

void UGameManager::PlayCutscene(const FString& CutsceneName)
{
	UE_LOG(LogTemp, Log, TEXT("Playing Cutscene: %s"), *CutsceneName);
	ChangeGameState(EGameState::Cutscene);

	// TODO: 컷신 재생 로직
	// 컷신 종료 후 자동으로 다음 상태로 전환
}

void UGameManager::StartBattle()
{
	UE_LOG(LogTemp, Log, TEXT("Starting Battle"));
	ChangeGameState(EGameState::InBattle);

	// TODO: BattleManager에게 전투 시작 알림
}

void UGameManager::EndBattle(bool bVictory)
{
	UE_LOG(LogTemp, Log, TEXT("Battle Ended - Victory: %s"), bVictory ? TEXT("True") : TEXT("False"));
	
	if (bVictory)
	{
		ChangeGameState(EGameState::BattleResult);
	}
	else
	{
		ChangeGameState(EGameState::GameOver);
	}
}

void UGameManager::GameOver()
{
	UE_LOG(LogTemp, Log, TEXT("Game Over"));
	ChangeGameState(EGameState::GameOver);

	// TODO: 게임 오버 화면 표시
}

void UGameManager::ReturnToMainMenu()
{
	UE_LOG(LogTemp, Log, TEXT("Returning to Main Menu"));
	ChangeGameState(EGameState::OutGame);

	// TODO: 메인 메뉴 레벨 로드
	// UGameplayStatics::OpenLevel(this, TEXT("MainMenu"));
}
