// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PaperFlipbook.h"
#include "CharacterData.generated.h"

/**
 * 캐릭터 등급
 */
UENUM(BlueprintType)
enum class ECharacterRarity : uint8
{
	Common UMETA(DisplayName = "Common"),
	Rare UMETA(DisplayName = "Rare"),
	SuperRare UMETA(DisplayName = "Super Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary")
};

/**
 * 팀 구분
 */
UENUM(BlueprintType)
enum class ETeam : uint8
{
	Ally UMETA(DisplayName = "Ally"),
	Enemy UMETA(DisplayName = "Enemy"),
	Neutral UMETA(DisplayName = "Neutral")
};

/**
 * 캐릭터 기본 스탯
 */
USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 AttackPower = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Defense = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AttackSpeed = 1.0f;  // 공격 속도 (AttackInterval의 역수)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MoveSpeed = 300.0f;
};

/**
 * 캐릭터 애니메이션 세트
 */
USTRUCT(BlueprintType)
struct FCharacterAnimations
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UPaperFlipbook> IdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UPaperFlipbook> WalkFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UPaperFlipbook> SlashFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UPaperFlipbook> DeadFlipbook;
};

/**
 * 캐릭터 데이터 애셋
 * - 캐릭터의 고유 정보를 정의
 * - 전투에서 재사용 가능
 */
UCLASS(BlueprintType)
class P1_API UCharacterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// ============================================
	// Data Import (맨 위에 표시)
	// ============================================
	
	// DataTable 레퍼런스 (에디터 전용)
	UPROPERTY(EditAnywhere, Category = "Data Import", meta = (AllowedClasses = "/Script/Engine.DataTable"))
	TSoftObjectPtr<UDataTable> SourceDataTable;

	// DataTable에서 스탯 임포트 (에디터 전용)
	UPROPERTY(EditAnywhere, Category = "Data Import", meta = (GetOptions = "GetDataTableRowNames"))
	FName DataTableRowName;

	// ============================================
	// Character Info (자동으로 채워짐)
	// ============================================
	
	// 캐릭터 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	FString CharacterName;

	// 캐릭터 설명
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (MultiLine = true))
	FText Description;

	// 등급
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	ECharacterRarity Rarity;

	// 기본 스탯
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	FCharacterStats BaseStats;

	// 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FCharacterAnimations Animations;

	// 캐릭터 아이콘 (UI용)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UTexture2D> Icon;

#if WITH_EDITOR
	// 프로퍼티 변경 감지
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// 드롭다운 옵션 제공
	UFUNCTION()
	TArray<FString> GetDataTableRowNames() const;
#endif

	// Primary Asset ID (DataAsset 식별용)
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("CharacterData", GetFName());
	}
};
