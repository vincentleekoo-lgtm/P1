// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "CharacterBase.generated.h"

class UPaperFlipbook;
class ABattleManager;

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
 * Idle, Walk, Slash, Dead 플립북 애니메이션 지원
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

	// 플립북 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UPaperFlipbook* IdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UPaperFlipbook* WalkFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UPaperFlipbook* SlashFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UPaperFlipbook* DeadFlipbook;

	// 현재 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	ECharacterState CurrentState;

	// 체력 시스템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 CurrentHealth;

	// 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 AttackPower;

	// 공격 주기 (초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackInterval;

	// 살아있는지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsAlive;

	// 사망 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnCharacterDied OnCharacterDied;

	// 현재 타겟 (public으로 변경하여 BattleManager가 접근 가능)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	ACharacterBase* CurrentTarget;

	// 상태 변경 함수
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetCharacterState(ECharacterState NewState);

	// 애니메이션 업데이트
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void UpdateAnimation();

	// 전투 시스템
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void TakeDamage(int32 DamageAmount, ACharacterBase* Attacker);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Attack(ACharacterBase* Target);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void StartAttacking();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void StopAttacking();

protected:
	// 상태에 맞는 플립북 가져오기
	UPaperFlipbook* GetFlipbookForState(ECharacterState State) const;

	// 공격 타이머
	FTimerHandle AttackTimerHandle;

	// 공격 실행
	void PerformAttack();
};
