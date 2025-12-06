// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterData.h"
#include "LobbyManager.generated.h"

/**
 * 전투 슬롯 정보
 */
USTRUCT(BlueprintType)
struct FBattleSlot
{
	GENERATED_BODY()

	// 슬롯에 배치된 캐릭터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Slot")
	TObjectPtr<UCharacterData> CharacterData;

	// 슬롯 위치 (0-4: 앞줄 왼쪽부터, 5-9: 뒷줄 왼쪽부터 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Slot")
	int32 Position;

	// 슬롯 활성화 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Slot")
	bool bIsActive;

	FBattleSlot()
		: CharacterData(nullptr)
		, Position(0)
		, bIsActive(false)
	{}
};

/**
 * 플레이어 덱 정보
 */
USTRUCT(BlueprintType)
struct FPlayerDeck
{
	GENERATED_BODY()

	// 소유한 캐릭터와 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck")
	TMap<TObjectPtr<UCharacterData>, int32> OwnedCharacters;

	FPlayerDeck()
	{
		OwnedCharacters.Empty();
	}
};

/**
 * 로비 매니저
 * - 플레이어 덱 관리
 * - 전투 편성 관리
 * - 전투 시작 전 준비
 */
UCLASS()
class P1_API ALobbyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ALobbyManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// ========== 플레이어 덱 ==========

	// 플레이어가 소유한 캐릭터 덱
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck")
	FPlayerDeck PlayerDeck;

	// 캐릭터 획득
	UFUNCTION(BlueprintCallable, Category = "Deck")
	void AddCharacterToDeck(UCharacterData* Character, int32 Count = 1);

	// 캐릭터 제거
	UFUNCTION(BlueprintCallable, Category = "Deck")
	void RemoveCharacterFromDeck(UCharacterData* Character, int32 Count = 1);

	// 캐릭터 소유 여부 확인
	UFUNCTION(BlueprintCallable, Category = "Deck")
	bool HasCharacter(UCharacterData* Character) const;

	// 캐릭터 소유 개수 확인
	UFUNCTION(BlueprintCallable, Category = "Deck")
	int32 GetCharacterCount(UCharacterData* Character) const;

	// ========== 전투 편성 ==========

	// 전투에 배치할 슬롯 (최대 5개)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Formation")
	TArray<FBattleSlot> BattleFormation;

	// 최대 슬롯 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Formation")
	int32 MaxSlots;

	// 슬롯에 캐릭터 배치
	UFUNCTION(BlueprintCallable, Category = "Battle Formation")
	bool AssignCharacterToSlot(int32 SlotIndex, UCharacterData* Character);

	// 슬롯에서 캐릭터 제거
	UFUNCTION(BlueprintCallable, Category = "Battle Formation")
	void ClearSlot(int32 SlotIndex);

	// 모든 슬롯 초기화
	UFUNCTION(BlueprintCallable, Category = "Battle Formation")
	void ClearAllSlots();

	// 편성 유효성 검사
	UFUNCTION(BlueprintCallable, Category = "Battle Formation")
	bool IsFormationValid() const;

	// 전투 준비 완료 캐릭터 목록 가져오기
	UFUNCTION(BlueprintCallable, Category = "Battle Formation")
	TArray<UCharacterData*> GetBattleReadyCharacters() const;

	// ========== UI 설정 ==========

	// 캐릭터 카드 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UCharacterCardWidget> CardWidgetClass;

	// 로비 UI 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class ULobbyUIWidget> LobbyUIClass;

	// ========== 전투 시작 ==========

	// 전투 시작 (StageManager에게 전달)
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void StartBattle();

	// 초기화
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void Initialize();

private:
	// 슬롯 초기화
	void InitializeSlots();
};
