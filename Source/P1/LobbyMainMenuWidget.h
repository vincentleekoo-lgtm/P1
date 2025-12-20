// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyMainMenuWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class UTextBlock;
class ALobbyManager;

/**
 * 로비 메뉴 카테고리
 */
UENUM(BlueprintType)
enum class ELobbyMenuCategory : uint8
{
	Battle UMETA(DisplayName = "Battle"),
	DeckManagement UMETA(DisplayName = "Deck Management"),
	Hunting UMETA(DisplayName = "Hunting"),
	Gathering UMETA(DisplayName = "Gathering")
};

/**
 * 로비 메인 메뉴 위젯
 * - 전투, 덱 관리, 사냥, 채집 카테고리 선택
 * - WidgetSwitcher로 카테고리별 UI 전환
 */
UCLASS()
class P1_API ULobbyMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// LobbyManager 설정
	UFUNCTION(BlueprintCallable, Category = "Lobby Main Menu")
	void SetLobbyManager(ALobbyManager* InLobbyManager);

	// 카테고리 전환
	UFUNCTION(BlueprintCallable, Category = "Lobby Main Menu")
	void SwitchCategory(ELobbyMenuCategory NewCategory);

protected:
	virtual void NativeConstruct() override;

	// 버튼 클릭 이벤트
	UFUNCTION()
	void OnBattleButtonClicked();

	UFUNCTION()
	void OnDeckManagementButtonClicked();

	UFUNCTION()
	void OnHuntingButtonClicked();

	UFUNCTION()
	void OnGatheringButtonClicked();

	// UI 업데이트
	void UpdatePlayerInfo();

public:
	// ========== UI 요소들 (BindWidget) ==========

	// 카테고리 버튼들
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BattleButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DeckManagementButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> HuntingButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GatheringButton;

	// 콘텐츠 전환기
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> ContentSwitcher;

	// 플레이어 정보 (TopBar)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldAmountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StaminaText;

	// 덱 관리 위젯 (Slot 1에 배치됨, 선택적)
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class ULobbyUIWidget> DeckManagementWidget;

protected:
	// LobbyManager 참조
	UPROPERTY()
	TObjectPtr<ALobbyManager> LobbyManager;

	// 현재 선택된 카테고리
	UPROPERTY()
	ELobbyMenuCategory CurrentCategory;
};
