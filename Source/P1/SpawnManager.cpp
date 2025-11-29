// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnManager.h"
#include "PartyMember.h"
#include "Enemy.h"
#include "BattleManager.h"

ASpawnManager::ASpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;

	PartySpawnPoint = nullptr;
	EnemySpawnPoint = nullptr;
	WarriorClass = nullptr;
	OrcClass = nullptr;
	SpawnedWarrior = nullptr;
	SpawnedOrc = nullptr;
	BattleManager = nullptr;
}

void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();

	// StageManager가 호출할 때까지 대기
	// SpawnCharacters()는 수동으로 호출됨
}

void ASpawnManager::SpawnCharacters()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null!"));
		return;
	}

	// 아군 스폰 (왼쪽)
	if (WarriorClass && PartySpawnPoint)
	{
		FVector SpawnLocation = PartySpawnPoint->GetActorLocation();
		FRotator SpawnRotation = PartySpawnPoint->GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		SpawnedWarrior = World->SpawnActor<APartyMember>(WarriorClass, SpawnLocation, SpawnRotation, SpawnParams);
		
		if (SpawnedWarrior)
		{
			UE_LOG(LogTemp, Log, TEXT("Warrior spawned at %s"), *SpawnLocation.ToString());
			
			// BattleManager에 등록
			if (BattleManager)
			{
				BattleManager->RegisterPartyMember(SpawnedWarrior);
				
				// 사망 이벤트 연결
				SpawnedWarrior->OnCharacterDied.AddDynamic(BattleManager, &ABattleManager::OnCharacterDied);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn Warrior!"));
		}
	}
	else
	{
		if (!WarriorClass)
			UE_LOG(LogTemp, Warning, TEXT("WarriorClass is not set!"));
		if (!PartySpawnPoint)
			UE_LOG(LogTemp, Warning, TEXT("PartySpawnPoint is not set!"));
	}

	// 적군 스폰 (오른쪽)
	if (OrcClass && EnemySpawnPoint)
	{
		FVector SpawnLocation = EnemySpawnPoint->GetActorLocation();
		FRotator SpawnRotation = EnemySpawnPoint->GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		SpawnedOrc = World->SpawnActor<AEnemy>(OrcClass, SpawnLocation, SpawnRotation, SpawnParams);
		
		if (SpawnedOrc)
		{
			UE_LOG(LogTemp, Log, TEXT("Orc spawned at %s"), *SpawnLocation.ToString());
			
			// BattleManager에 등록
			if (BattleManager)
			{
				BattleManager->RegisterEnemy(SpawnedOrc);
				
				// 사망 이벤트 연결
				SpawnedOrc->OnCharacterDied.AddDynamic(BattleManager, &ABattleManager::OnCharacterDied);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn Orc!"));
		}
	}
	else
	{
		if (!OrcClass)
			UE_LOG(LogTemp, Warning, TEXT("OrcClass is not set!"));
		if (!EnemySpawnPoint)
			UE_LOG(LogTemp, Warning, TEXT("EnemySpawnPoint is not set!"));
	}
}
