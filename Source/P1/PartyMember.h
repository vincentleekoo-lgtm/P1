// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "PartyMember.generated.h"

/**
 * 아군 캐릭터 클래스
 * 플레이어가 조종하거나 동료로 사용하는 캐릭터
 */
UCLASS()
class P1_API APartyMember : public ACharacterBase
{
	GENERATED_BODY()

public:
	APartyMember();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// 아군 고유 속성
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
	FString CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
	int32 Level;
};
