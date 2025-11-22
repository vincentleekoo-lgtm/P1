// Fill out your copyright notice in the Description page of Project Settings.

#include "PartyMember.h"

APartyMember::APartyMember()
{
	PrimaryActorTick.bCanEverTick = true;

	// 기본 스탯 설정
	CharacterName = TEXT("Party Member");
	Level = 1;
	MaxHealth = 100;
	Health = MaxHealth;
}

void APartyMember::BeginPlay()
{
	Super::BeginPlay();
}

void APartyMember::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
