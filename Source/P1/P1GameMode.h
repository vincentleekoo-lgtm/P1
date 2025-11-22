// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P1GameMode.generated.h"

class APartyMember;
class AEnemy;
class ATargetPoint;

/**
 * P1 게임 모드
 * 게임 시작 시 아군과 적군을 스폰
 */
UCLASS()
class P1_API AP1GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AP1GameMode();

protected:
	virtual void BeginPlay() override;

public:
	// 아군 스폰 위치 (왼쪽)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	AActor* PartySpawnPoint;

	// 적군 스폰 위치 (오른쪽)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	AActor* EnemySpawnPoint;

	// 스폰할 아군 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<APartyMember> WarriorClass;

	// 스폰할 적군 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AEnemy> OrcClass;

	// 스폰 함수
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnCharacters();

private:
	APartyMember* SpawnedWarrior;
	AEnemy* SpawnedOrc;
};
