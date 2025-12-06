// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterData.h"
#include "CharacterDataTableRow.generated.h"

/**
 * DataTable Row 구조체
 * CSV 파일에서 임포트하여 사용
 */
USTRUCT(BlueprintType)
struct FCharacterDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	// 캐릭터 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FString CharacterName;

	// 등급 (문자열로 받아서 Enum으로 변환)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FString RarityType;

	// 기본 스탯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MoveSpeed;

	// 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FString Description;

	FCharacterDataTableRow()
		: MaxHealth(100)
		, AttackPower(10)
		, Defense(0)
		, AttackSpeed(1.0f)
		, MoveSpeed(300.0f)
	{}

	// 문자열을 ECharacterRarity로 변환
	ECharacterRarity GetRarityEnum() const
	{
		if (RarityType == TEXT("Common"))
			return ECharacterRarity::Common;
		else if (RarityType == TEXT("Rare"))
			return ECharacterRarity::Rare;
		else if (RarityType == TEXT("SuperRare"))
			return ECharacterRarity::SuperRare;
		else if (RarityType == TEXT("Epic"))
			return ECharacterRarity::Epic;
		else if (RarityType == TEXT("Legendary"))
			return ECharacterRarity::Legendary;
		
		return ECharacterRarity::Common;
	}

	// DataTable Row를 CharacterData로 변환 (애니메이션은 별도 설정 필요)
	void FillCharacterData(UCharacterData* OutData) const
	{
		if (!OutData)
			return;

		OutData->CharacterName = CharacterName;
		OutData->Description = FText::FromString(Description);
		OutData->Rarity = GetRarityEnum();
		
		OutData->BaseStats.MaxHealth = MaxHealth;
		OutData->BaseStats.AttackPower = AttackPower;
		OutData->BaseStats.Defense = Defense;
		OutData->BaseStats.AttackSpeed = AttackSpeed;
		OutData->BaseStats.MoveSpeed = MoveSpeed;
	}
};
