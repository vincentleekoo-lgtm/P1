// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "PaperFlipbookComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 기본 상태 설정
	CurrentState = ECharacterState::Idle;
	Team = ETeam::Neutral;
	CharacterData = nullptr;

	// 전투 속성 초기화 (DataAsset에서 로드될 예정)
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
	
	// CharacterData가 있으면 자동 초기화
	if (CharacterData)
	{
		InitializeFromData(CharacterData, Team);
	}
	
	// 시작 시 애니메이션 업데이트
	UpdateAnimation();
}

void ACharacterBase::InitializeFromData(UCharacterData* Data, ETeam AssignedTeam)
{
	if (!Data)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: CharacterData is null!"), *GetName());
		return;
	}

	CharacterData = Data;
	Team = AssignedTeam;

	// 스탯 로드
	MaxHealth = Data->BaseStats.MaxHealth;
	CurrentHealth = MaxHealth;
	AttackPower = Data->BaseStats.AttackPower;
	
	// AttackSpeed를 AttackInterval로 변환 (AttackSpeed가 높을수록 빠름)
	AttackInterval = Data->BaseStats.AttackSpeed > 0 ? (1.0f / Data->BaseStats.AttackSpeed) : 2.0f;

	bIsAlive = true;

	UE_LOG(LogTemp, Log, TEXT("%s initialized: %s (Team: %d, HP: %d, ATK: %d)"), 
		*GetName(), *Data->CharacterName, static_cast<int32>(Team), MaxHealth, AttackPower);
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
	if (!CharacterData)
		return nullptr;

	switch (State)
	{
	case ECharacterState::Idle:
		return CharacterData->Animations.IdleFlipbook;
	case ECharacterState::Walk:
		return CharacterData->Animations.WalkFlipbook;
	case ECharacterState::Slash:
		return CharacterData->Animations.SlashFlipbook;
	case ECharacterState::Dead:
		return CharacterData->Animations.DeadFlipbook;
	default:
		return CharacterData->Animations.IdleFlipbook;
	}
}

void ACharacterBase::ApplyDamage(int32 DamageAmount, ACharacterBase* Attacker)
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
	Target->ApplyDamage(AttackPower, this);

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

	// 이 캐릭터를 타겟으로 하는 다른 캐릭터들의 공격 중지
	// (BattleManager가 처리하도록 이벤트만 발행)
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
		UE_LOG(LogTemp, Warning, TEXT("%s stopping attack - target invalid or dead"), *GetName());
		StopAttacking();
		CurrentTarget = nullptr;
		return;
	}

	Attack(CurrentTarget);
}
