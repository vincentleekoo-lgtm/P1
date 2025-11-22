// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "PaperFlipbookComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 기본 상태 설정
	CurrentState = ECharacterState::Idle;

	// 플립북 초기화
	IdleFlipbook = nullptr;
	WalkFlipbook = nullptr;
	SlashFlipbook = nullptr;
	DeadFlipbook = nullptr;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	// 시작 시 애니메이션 업데이트
	UpdateAnimation();
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterBase::SetCharacterState(ECharacterState NewState)
{
	if (CurrentState != NewState)
	{
		CurrentState = NewState;
		UpdateAnimation();
	}
}

void ACharacterBase::UpdateAnimation()
{
	UPaperFlipbook* TargetFlipbook = GetFlipbookForState(CurrentState);
	
	if (TargetFlipbook && GetSprite())
	{
		GetSprite()->SetFlipbook(TargetFlipbook);
	}
}

UPaperFlipbook* ACharacterBase::GetFlipbookForState(ECharacterState State) const
{
	switch (State)
	{
	case ECharacterState::Idle:
		return IdleFlipbook;
	case ECharacterState::Walk:
		return WalkFlipbook;
	case ECharacterState::Slash:
		return SlashFlipbook;
	case ECharacterState::Dead:
		return DeadFlipbook;
	default:
		return IdleFlipbook;
	}
}
