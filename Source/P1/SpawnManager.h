// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

class APartyMember;
class AEnemy;

/**
 * 캐릭터 스폰 매니저
 * 월드에 배치 가능한 액터로, TargetPoint를 직접 할당 가능
 */
UCLASS()
class P1_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnManager();

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
