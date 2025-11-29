// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleManager.h"
#include "PartyMember.h"
#include "Enemy.h"

ABattleManager::ABattleManager()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentBattleState = EBattleState::NotStarted;
	bIsInitialized = false;
}

void ABattleManager::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("===== BattleManager Created - Name: %s ====="), *GetName());
}

void ABattleManager::Initialize()
{
	if (bIsInitialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("BattleManager already initialized"));
		return;
	}

	bIsInitialized = true;
	UE_LOG(LogTemp, Log, TEXT("BattleManager Initialized"));
}

void ABattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 전투 진행 중일 때만 조건 체크
	if (CurrentBattleState == EBattleState::InProgress)
	{
		// 매 프레임마다가 아닌 주기적으로 체크하도록 최적화 가능
		CheckBattleConditions();
	}
}

void ABattleManager::ChangeBattleState(EBattleState NewState)
{
	if (CurrentBattleState == NewState)
		return;

	EBattleState OldState = CurrentBattleState;
	CurrentBattleState = NewState;

	UE_LOG(LogTemp, Log, TEXT("BattleState Changed: %d -> %d"), static_cast<int32>(OldState), static_cast<int32>(NewState));
}

void ABattleManager::StartBattle()
{
	UE_LOG(LogTemp, Log, TEXT("Battle Started - Party: %d, Enemies: %d"), PartyMembers.Num(), Enemies.Num());
	
	ChangeBattleState(EBattleState::InProgress);
	OnBattleStarted.Broadcast();

	// 전투 시작: 모든 캐릭터에게 타겟 할당 및 공격 시작
	AssignTargetsAndStartCombat();
}

void ABattleManager::AssignTargetsAndStartCombat()
{
	// 아군 -> 첫 번째 적 타겟팅
	if (Enemies.Num() > 0)
	{
		for (APartyMember* Member : PartyMembers)
		{
			if (Member && Member->bIsAlive)
			{
				Member->CurrentTarget = Enemies[0];
				Member->StartAttacking();
			}
		}
	}

	// 적군 -> 첫 번째 아군 타겟팅
	if (PartyMembers.Num() > 0)
	{
		for (AEnemy* Enemy : Enemies)
		{
			if (Enemy && Enemy->bIsAlive)
			{
				Enemy->CurrentTarget = PartyMembers[0];
				Enemy->StartAttacking();
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Combat targets assigned and attacks started"));
}

void ABattleManager::EndBattle(bool bVictory)
{
	UE_LOG(LogTemp, Log, TEXT("Battle Ended - Victory: %s"), bVictory ? TEXT("True") : TEXT("False"));

	// 모든 캐릭터의 공격 중지
	for (APartyMember* Member : PartyMembers)
	{
		if (Member)
		{
			Member->StopAttacking();
		}
	}

	for (AEnemy* Enemy : Enemies)
	{
		if (Enemy)
		{
			Enemy->StopAttacking();
		}
	}

	ChangeBattleState(bVictory ? EBattleState::Victory : EBattleState::Defeat);
	OnBattleEnded.Broadcast(bVictory);
	
	// Delegate로 결과 알림 (StageManager가 구독)
	OnBattleCompleted.Broadcast(bVictory);
}

void ABattleManager::PauseBattle()
{
	if (CurrentBattleState == EBattleState::InProgress)
	{
		UE_LOG(LogTemp, Log, TEXT("Battle Paused"));
		ChangeBattleState(EBattleState::Paused);
		
		// TODO: 게임 일시정지 처리
	}
}

void ABattleManager::ResumeBattle()
{
	if (CurrentBattleState == EBattleState::Paused)
	{
		UE_LOG(LogTemp, Log, TEXT("Battle Resumed"));
		ChangeBattleState(EBattleState::InProgress);
		
		// TODO: 게임 재개 처리
	}
}

void ABattleManager::RegisterPartyMember(APartyMember* Member)
{
	if (Member && !PartyMembers.Contains(Member))
	{
		PartyMembers.Add(Member);
		UE_LOG(LogTemp, Log, TEXT("PartyMember registered: %s (Total: %d)"), *Member->CharacterName, PartyMembers.Num());
	}
}

void ABattleManager::RegisterEnemy(AEnemy* Enemy)
{
	if (Enemy && !Enemies.Contains(Enemy))
	{
		Enemies.Add(Enemy);
		UE_LOG(LogTemp, Log, TEXT("Enemy registered: %s (Total: %d)"), *Enemy->EnemyType, Enemies.Num());
	}
}

void ABattleManager::UnregisterPartyMember(APartyMember* Member)
{
	if (PartyMembers.Contains(Member))
	{
		PartyMembers.Remove(Member);
		UE_LOG(LogTemp, Log, TEXT("PartyMember unregistered (Remaining: %d)"), PartyMembers.Num());
	}
}

void ABattleManager::UnregisterEnemy(AEnemy* Enemy)
{
	if (Enemies.Contains(Enemy))
	{
		Enemies.Remove(Enemy);
		UE_LOG(LogTemp, Log, TEXT("Enemy unregistered (Remaining: %d)"), Enemies.Num());
	}
}

void ABattleManager::OnCharacterDied(ACharacterBase* Character)
{
	if (!Character)
		return;

	UE_LOG(LogTemp, Log, TEXT("Character Died: %s"), *Character->GetName());
	OnCharacterDefeated.Broadcast(Character);

	// PartyMember인지 Enemy인지 확인하고 리스트에서 제거
	if (APartyMember* PartyMember = Cast<APartyMember>(Character))
	{
		UnregisterPartyMember(PartyMember);
	}
	else if (AEnemy* Enemy = Cast<AEnemy>(Character))
	{
		UnregisterEnemy(Enemy);
	}

	// 사망 후 승패 조건 체크
	CheckBattleConditions();
}

void ABattleManager::CheckBattleConditions()
{
	// 전투 진행 중이 아니면 체크하지 않음
	if (CurrentBattleState != EBattleState::InProgress)
		return;

	// 적 전멸 -> 승리
	if (Enemies.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("All enemies defeated - Victory!"));
		EndBattle(true);
		return;
	}

	// 아군 전멸 -> 패배
	if (PartyMembers.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("All party members defeated - Defeat!"));
		EndBattle(false);
		return;
	}
}
