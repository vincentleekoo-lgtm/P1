// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// 기본 스탯 설정
	EnemyType = TEXT("Enemy");
	DetectionRange = 500.0f;
	
	// CharacterBase의 전투 속성 오버라이드
	MaxHealth = 50;
	CurrentHealth = MaxHealth;
	AttackPower = 10;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
