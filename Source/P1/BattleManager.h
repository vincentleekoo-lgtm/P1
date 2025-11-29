// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleManager.generated.h"

class APartyMember;
class AEnemy;

UENUM(BlueprintType)
enum class EBattleState : uint8
{
	NotStarted UMETA(DisplayName = "NotStarted"),
	Preparing UMETA(DisplayName = "Preparing"),
	InProgress UMETA(DisplayName = "InProgress"),
	Victory UMETA(DisplayName = "Victory"),
	Defeat UMETA(DisplayName = "Defeat"),
	Paused UMETA(DisplayName = "Paused")
};

// 전투 이벤트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleEnded, bool, bVictory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDefeated, AActor*, DefeatedCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleCompleted, bool, bVictory);

/**
 * 전투 관리자
 * - 전투 흐름 제어
 * - 턴/실시간 전투 관리
 * - 승패 판정
 * - 전투 참가자 추적
 */
UCLASS()
class P1_API ABattleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ABattleManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 전투 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle")
	EBattleState CurrentBattleState;

	// 전투 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Battle Events")
	FOnBattleStarted OnBattleStarted;

	UPROPERTY(BlueprintAssignable, Category = "Battle Events")
	FOnBattleEnded OnBattleEnded;

	UPROPERTY(BlueprintAssignable, Category = "Battle Events")
	FOnCharacterDefeated OnCharacterDefeated;

	// 전투 완료 이벤트 (StageManager에게 알림용)
	UPROPERTY(BlueprintAssignable, Category = "Battle Events")
	FOnBattleCompleted OnBattleCompleted;

	// 전투 참가자 리스트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle")
	TArray<APartyMember*> PartyMembers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle")
	TArray<AEnemy*> Enemies;

	// 전투 시작
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void StartBattle();

	// 전투 종료
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void EndBattle(bool bVictory);

	// 전투 일시정지/재개
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void PauseBattle();

	UFUNCTION(BlueprintCallable, Category = "Battle")
	void ResumeBattle();

	// 참가자 등록
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void RegisterPartyMember(APartyMember* Member);

	UFUNCTION(BlueprintCallable, Category = "Battle")
	void RegisterEnemy(AEnemy* Enemy);

	// 참가자 제거
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void UnregisterPartyMember(APartyMember* Member);

	UFUNCTION(BlueprintCallable, Category = "Battle")
	void UnregisterEnemy(AEnemy* Enemy);

	// 캐릭터 사망 처리
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void OnCharacterDied(AActor* Character);

	// 승패 체크
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void CheckBattleConditions();

private:
	void ChangeBattleState(EBattleState NewState);
};
