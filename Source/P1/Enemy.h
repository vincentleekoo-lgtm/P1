// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Enemy.generated.h"

/**
 * 적 캐릭터 클래스
 * 플레이어와 적대하는 캐릭터
 */
UCLASS()
class P1_API AEnemy : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// 적 고유 속성
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	FString EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float DetectionRange;
};
