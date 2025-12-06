// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUIWidget.generated.h"

class UScrollBox;
class UButton;
class UTextBlock;
class UCharacterCardWidget;
class UDeckSlotWidget;
class ALobbyManager;
class UCharacterData;

/**
 * 로비 메인 UI
 * - 캐릭터 목록 표시
 * - 덱 편성 슬롯 표시
 * - 전투 시작 버튼
 */
UCLASS()
class P1_API ULobbyUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// LobbyManager 설정
	UFUNCTION(BlueprintCallable, Category = "Lobby UI")
	void SetLobbyManager(ALobbyManager* InLobbyManager);

	// UI 초기화
	UFUNCTION(BlueprintCallable, Category = "Lobby UI")
	void InitializeUI();

	// 캐릭터 목록 갱신
	UFUNCTION(BlueprintCallable, Category = "Lobby UI")
	void RefreshCharacterList();

	// 덱 슬롯 갱신
	UFUNCTION(BlueprintCallable, Category = "Lobby UI")
	void RefreshDeckSlots();

protected:
	virtual void NativeConstruct() override;

	// 캐릭터 카드 클릭 이벤트
	UFUNCTION()
	void OnCharacterCardClicked(UCharacterCardWidget* CardWidget);

	// 덱 슬롯 클릭 이벤트
	UFUNCTION()
	void OnDeckSlotClicked(UDeckSlotWidget* SlotWidget);

	// 전투 시작 버튼 클릭
	UFUNCTION()
	void OnStartBattleClicked();

public:
	// UI 요소들
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> CharacterListScrollBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartBattleButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> FormationStatusText;

	// 덱 슬롯들 (5개)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDeckSlotWidget> DeckSlot_0;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDeckSlotWidget> DeckSlot_1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDeckSlotWidget> DeckSlot_2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDeckSlotWidget> DeckSlot_3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDeckSlotWidget> DeckSlot_4;

	// 캐릭터 카드 위젯 클래스 (Blueprint에서 설정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby UI")
	TSubclassOf<UCharacterCardWidget> CharacterCardWidgetClass;

protected:
	// LobbyManager 참조
	UPROPERTY()
	TObjectPtr<ALobbyManager> LobbyManager;

	// 생성된 캐릭터 카드들
	UPROPERTY()
	TArray<TObjectPtr<UCharacterCardWidget>> CharacterCards;

	// 덱 슬롯 배열
	UPROPERTY()
	TArray<TObjectPtr<UDeckSlotWidget>> DeckSlots;

	// 현재 선택된 캐릭터
	UPROPERTY()
	TObjectPtr<UCharacterData> SelectedCharacter;

	// 현재 선택된 슬롯
	UPROPERTY()
	TObjectPtr<UDeckSlotWidget> SelectedSlot;
};
