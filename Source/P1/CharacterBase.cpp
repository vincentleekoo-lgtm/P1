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

	// 전투 속성 초기화
	MaxHealth = 100;
	CurrentHealth = MaxHealth;
	AttackPower = 10;
	AttackInterval = 2.0f;
	bIsAlive = true;
	CurrentTarget = nullptr;
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

void ACharacterBase::TakeDamage(int32 DamageAmount, ACharacterBase* Attacker)
{
	if (!bIsAlive)
		return;

	CurrentHealth -= DamageAmount;
	UE_LOG(LogTemp, Log, TEXT("%s took %d damage. Health: %d/%d"), *GetName(), DamageAmount, CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		Die();
	}
}

void ACharacterBase::Attack(ACharacterBase* Target)
{
	if (!bIsAlive || !Target || !Target->bIsAlive)
		return;

	// 공격 애니메이션
	SetCharacterState(ECharacterState::Slash);

	// 데미지 적용
	Target->TakeDamage(AttackPower, this);

	UE_LOG(LogTemp, Log, TEXT("%s attacks %s for %d damage"), *GetName(), *Target->GetName(), AttackPower);

	// 애니메이션 후 Idle로 복귀 (타이머로 처리 가능)
	FTimerHandle AnimTimer;
	GetWorld()->GetTimerManager().SetTimer(AnimTimer, [this]()
	{
		if (bIsAlive)
		{
			SetCharacterState(ECharacterState::Idle);
		}
	}, 0.5f, false);
}

void ACharacterBase::Die()
{
	if (!bIsAlive)
		return;

	bIsAlive = false;
	SetCharacterState(ECharacterState::Dead);
	
	// 공격 중지
	StopAttacking();

	UE_LOG(LogTemp, Log, TEXT("%s has died"), *GetName());

	// 사망 이벤트 브로드캐스트
	OnCharacterDied.Broadcast(this);

	// TODO: 사망 애니메이션 후 액터 제거 또는 비활성화
}

void ACharacterBase::StartAttacking()
{
	if (!bIsAlive || !CurrentTarget)
		return;

	UE_LOG(LogTemp, Log, TEXT("%s started attacking %s"), *GetName(), *CurrentTarget->GetName());

	// 주기적으로 공격
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ACharacterBase::PerformAttack, AttackInterval, true, 0.0f);
}

void ACharacterBase::StopAttacking()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}

void ACharacterBase::PerformAttack()
{
	if (!bIsAlive || !CurrentTarget || !CurrentTarget->bIsAlive)
	{
		StopAttacking();
		return;
	}

	Attack(CurrentTarget);
}
