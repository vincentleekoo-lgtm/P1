// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "CharacterBase.generated.h"

class UPaperFlipbook;

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

	// 상태 변경 함수
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetCharacterState(ECharacterState NewState);

	// 애니메이션 업데이트
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void UpdateAnimation();

protected:
	// 상태에 맞는 플립북 가져오기
	UPaperFlipbook* GetFlipbookForState(ECharacterState State) const;
};
