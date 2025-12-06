// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "CharacterData.h"
#include "CharacterBase.generated.h"

class UPaperFlipbook;
class ABattleManager;
class UCharacterData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDied, ACharacterBase*, Character);

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Walk UMETA(DisplayName = "Walk"),
	Slash UMETA(DisplayName = "Slash"),
	Dead UMETA(DisplayName = "Dead")
};

/**
 * 2D 캐릭터 베이스 클래스
 * - CharacterData (DataAsset) 기반
 * - Team 속성으로 아군/적군 구분
 * - 같은 캐릭터를 양쪽 팀에서 사용 가능
 */
UCLASS()
class P1_API ACharacterBase : public APaperCharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ========== 캐릭터 데이터 ==========
	
	// 캐릭터 정보 (DataAsset)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TObjectPtr<UCharacterData> CharacterData;

	// 소속 팀
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	ETeam Team;

	// 캐릭터 초기화 (DataAsset에서 스탯 로드)
	UFUNCTION(BlueprintCallable, Category = "Character")
	void InitializeFromData(UCharacterData* Data, ETeam AssignedTeam);

	// ========== 애니메이션 ==========

	// 현재 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	ECharacterState CurrentState;

	// 상태 변경 함수
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetCharacterState(ECharacterState NewState);

	// 애니메이션 업데이트
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void UpdateAnimation();

	// ========== 전투 스탯 ==========

	// ========== 전투 스탯 ==========

	// 체력 (전투 중 변동)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 CurrentHealth;

	// 공격력 (DataAsset에서 로드)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 AttackPower;

	// 공격 주기 (초) - AttackSpeed에서 계산
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackInterval;

	// 살아있는지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsAlive;

	// ========== 전투 시스템 ==========

	// ========== 전투 시스템 ==========

	// 사망 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnCharacterDied OnCharacterDied;

	// 현재 타겟
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	ACharacterBase* CurrentTarget;

	// 전투 함수
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void ApplyDamage(int32 DamageAmount, ACharacterBase* Attacker);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Attack(ACharacterBase* Target);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void StartAttacking();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void StopAttacking();

protected:
	// DataAsset에서 플립북 가져오기
	UPaperFlipbook* GetFlipbookForState(ECharacterState State) const;

	// 공격 타이머
	FTimerHandle AttackTimerHandle;

	// 공격 실행
	void PerformAttack();
};
