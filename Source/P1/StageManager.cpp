// Fill out your copyright notice in the Description page of Project Settings.

#include "StageManager.h"
#include "SpawnManager.h"
#include "BattleManager.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"

AStageManager::AStageManager()
{
	PrimaryActorTick.bCanEverTick = true;

	StageNumber = 1;
	StageName = TEXT("Stage 1");
	CurrentStageState = EStageState::NotStarted;
	TotalWaves = 1;
	CurrentWave = 0;
	bAutoStartOnBeginPlay = true;  // 기본값: 자동 시작

	SpawnManager = nullptr;
	BattleManager = nullptr;
}

void AStageManager::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Log, TEXT("StageManager Created - Stage: %s"), *StageName);
	
	// BattleManager 이벤트 구독
	if (BattleManager)
	{
		BattleManager->OnBattleCompleted.AddDynamic(this, &AStageManager::HandleBattleCompleted);
		UE_LOG(LogTemp, Log, TEXT("Subscribed to BattleManager events"));
	}
	
	// 자동 시작
	if (bAutoStartOnBeginPlay)
	{
		UE_LOG(LogTemp, Log, TEXT("Auto-starting stage..."));
		StartStage();
	}
}

void AStageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStageManager::ChangeStageState(EStageState NewState)
{
	if (CurrentStageState == NewState)
		return;

	EStageState OldState = CurrentStageState;
	CurrentStageState = NewState;

	UE_LOG(LogTemp, Log, TEXT("StageState Changed: %d -> %d"), static_cast<int32>(OldState), static_cast<int32>(NewState));
}

void AStageManager::StartStage()
{
	UE_LOG(LogTemp, Log, TEXT("Starting Stage: %s"), *StageName);
	ChangeStageState(EStageState::Preparing);

	// GameManager에게 전투 시작 알림
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UGameManager* GameManager = GameInstance->GetSubsystem<UGameManager>())
		{
			GameManager->StartBattle();
		}
	}

	// 첫 웨이브 시작
	CurrentWave = 0;
	StartNextWave();
}

void AStageManager::StartNextWave()
{
	CurrentWave++;

	if (CurrentWave > TotalWaves)
	{
		UE_LOG(LogTemp, Log, TEXT("All waves completed!"));
		CompleteStage();
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Starting Wave %d / %d"), CurrentWave, TotalWaves);
	ChangeStageState(EStageState::InProgress);

	// SpawnManager를 통해 적 스폰
	if (SpawnManager)
	{
		SpawnManager->SpawnCharacters();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnManager is not set!"));
	}
}

void AStageManager::CompleteStage()
{
	UE_LOG(LogTemp, Log, TEXT("Stage Completed: %s"), *StageName);
	ChangeStageState(EStageState::Completed);

	// GameManager에게 승리 알림
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UGameManager* GameManager = GameInstance->GetSubsystem<UGameManager>())
		{
			GameManager->EndBattle(true);
		}
	}
}

void AStageManager::FailStage()
{
	UE_LOG(LogTemp, Log, TEXT("Stage Failed: %s"), *StageName);
	ChangeStageState(EStageState::Failed);

	// GameManager에게 패배 알림
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UGameManager* GameManager = GameInstance->GetSubsystem<UGameManager>())
		{
			GameManager->EndBattle(false);
		}
	}
}

void AStageManager::HandleBattleCompleted(bool bVictory)
{
	UE_LOG(LogTemp, Log, TEXT("HandleBattleCompleted called - Victory: %s"), bVictory ? TEXT("True") : TEXT("False"));
	
	if (bVictory)
	{
		CompleteStage();
	}
	else
	{
		FailStage();
	}
}
