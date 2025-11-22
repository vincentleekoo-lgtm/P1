// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// 기본 스탯 설정
	EnemyType = TEXT("Enemy");
	MaxHealth = 50;
	Health = MaxHealth;
	AttackPower = 10;
	DetectionRange = 500.0f;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
