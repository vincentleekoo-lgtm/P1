// Fill out your copyright notice in the Description page of Project Settings.

#include "P1GameMode.h"
#include "PartyMember.h"
#include "Enemy.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

AP1GameMode::AP1GameMode()
{
	// DefaultPawn 자동 스폰 비활성화
	DefaultPawnClass = nullptr;
	
	PartySpawnPoint = nullptr;
	EnemySpawnPoint = nullptr;
	WarriorClass = nullptr;
	OrcClass = nullptr;
	SpawnedWarrior = nullptr;
	SpawnedOrc = nullptr;
}

void AP1GameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("P1GameMode started - using SpawnManager for character spawning"));
	
	// SpawnManager가 캐릭터 스폰 담당
	// 자동 스폰 비활성화
}

void AP1GameMode::SpawnCharacters()
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
