// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleMenuWidget.generated.h"

class UImage;
class UCanvasPanel;
class UTextBlock;
class UHorizontalBox;
class UButton;
class UStageButtonWidget;
class UStageData;
class ALobbyManager;

/**
 * 전투 시작 메뉴 - 월드맵 + 스테이지 선택
 */
UCLASS()
class P1_API UBattleMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// LobbyManager 설정
	UFUNCTION(BlueprintCallable, Category = "Battle Menu")
	void SetLobbyManager(ALobbyManager* InLobbyManager);

	// 스테이지 선택
	UFUNCTION(BlueprintCallable, Category = "Battle Menu")
	void SelectStage(UStageData* Stage);

	// 전투 시작
	UFUNCTION(BlueprintCallable, Category = "Battle Menu")
	void StartBattle();

protected:
	virtual void NativeConstruct() override;

	// UI 초기화
	void InitializeStageButtons();
	void UpdateDeckPreview();
	void UpdateStageInfo();

	// 버튼 이벤트
	UFUNCTION()
	void OnStartBattleButtonClicked();

	UFUNCTION()
	void OnStageButtonClicked(UStageData* StageData);

public:
	// ========== UI 요소들 ==========

	// 미니맵
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MinimapImage;

	// 스테이지 버튼 컨테이너 (Canvas Panel)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> StageButtonContainer;

	// 선택된 스테이지 정보 패널
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StageNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StageLevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StageDescriptionText;

	// 덱 미리보기 (현재 편성된 캐릭터들)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> DeckPreviewBox;

	// 전투 시작 버튼
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartBattleButton;

	// 스테이지 버튼 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UStageButtonWidget> StageButtonWidgetClass;

protected:
	// LobbyManager 참조
	UPROPERTY()
	TObjectPtr<ALobbyManager> LobbyManager;

	// 선택된 스테이지
	UPROPERTY()
	TObjectPtr<UStageData> SelectedStage;

	// 생성된 스테이지 버튼들
	UPROPERTY()
	TArray<TObjectPtr<UStageButtonWidget>> StageButtons;
};
