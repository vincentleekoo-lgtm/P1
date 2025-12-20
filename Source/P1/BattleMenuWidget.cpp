// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleMenuWidget.h"
#include "LobbyManager.h"
#include "StageData.h"
#include "StageButtonWidget.h"
#include "CharacterData.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"

void UBattleMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("[BattleMenuWidget] NativeConstruct called"));

	// 전투 시작 버튼 바인딩
	if (StartBattleButton)
	{
		StartBattleButton->OnClicked.AddDynamic(this, &UBattleMenuWidget::OnStartBattleButtonClicked);
		StartBattleButton->SetIsEnabled(false); // 스테이지 선택 전까지 비활성화
	}
}

void UBattleMenuWidget::SetLobbyManager(ALobbyManager* InLobbyManager)
{
	UE_LOG(LogTemp, Warning, TEXT("[BattleMenuWidget] SetLobbyManager called"));

	LobbyManager = InLobbyManager;

	if (LobbyManager)
	{
		InitializeStageButtons();
		UpdateDeckPreview();
		UE_LOG(LogTemp, Log, TEXT("[BattleMenuWidget] LobbyManager set successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[BattleMenuWidget] LobbyManager is NULL!"));
	}
}

void UBattleMenuWidget::InitializeStageButtons()
{
	if (!LobbyManager || !StageButtonContainer || !StageButtonWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[BattleMenuWidget] Cannot initialize stage buttons - missing references"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[BattleMenuWidget] Initializing stage buttons..."));

	// 기존 버튼들 제거
	StageButtonContainer->ClearChildren();
	StageButtons.Empty();

	// LobbyManager에서 스테이지 목록 가져오기
	if (LobbyManager->AvailableStages.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BattleMenuWidget] No available stages in LobbyManager"));
		return;
	}

	// 각 스테이지마다 버튼 생성
	for (int32 i = 0; i < LobbyManager->AvailableStages.Num(); i++)
	{
		UStageData* StageData = LobbyManager->AvailableStages[i];
		if (!StageData)
		{
			continue;
		}

		// 스테이지 버튼 위젯 생성
		UStageButtonWidget* StageButton = CreateWidget<UStageButtonWidget>(this, StageButtonWidgetClass);
		if (StageButton)
		{
			StageButton->SetStageData(StageData);
			StageButton->OnStageButtonClickedEvent.AddDynamic(this, &UBattleMenuWidget::OnStageButtonClicked);

			// Canvas Panel에 추가 - 자동 배치 (Grid 형태)
			UCanvasPanelSlot* CanvasSlot = StageButtonContainer->AddChildToCanvas(StageButton);
			if (CanvasSlot)
			{
				// 자동 배치: 3x3 그리드 형태로 배치
				const float ButtonSize = 80.0f;
				const float Spacing = 20.0f;
				const int32 Columns = 3;

				int32 Row = i / Columns;
				int32 Col = i % Columns;

				FVector2D Position(
					100.0f + Col * (ButtonSize + Spacing),
					100.0f + Row * (ButtonSize + Spacing)
				);

				CanvasSlot->SetPosition(Position);
				CanvasSlot->SetSize(FVector2D(ButtonSize, ButtonSize));
			}

			StageButtons.Add(StageButton);
			UE_LOG(LogTemp, Log, TEXT("[BattleMenuWidget] Created stage button %d: %s"), 
				i, *StageData->StageName);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("[BattleMenuWidget] Initialized %d stage buttons"), StageButtons.Num());
}

void UBattleMenuWidget::UpdateDeckPreview()
{
	if (!LobbyManager || !DeckPreviewBox)
	{
		return;
	}

	// 덱 프리뷰 업데이트 (현재 편성된 캐릭터 표시)
	// TODO: 캐릭터 아이콘 표시
	UE_LOG(LogTemp, Log, TEXT("[BattleMenuWidget] Deck preview updated"));
}

void UBattleMenuWidget::SelectStage(UStageData* Stage)
{
	SelectedStage = Stage;
	UpdateStageInfo();

	// 전투 시작 버튼 활성화
	if (StartBattleButton)
	{
		StartBattleButton->SetIsEnabled(true);
	}

	UE_LOG(LogTemp, Log, TEXT("[BattleMenuWidget] Stage selected: %s"), *Stage->StageName);
}

void UBattleMenuWidget::UpdateStageInfo()
{
	if (!SelectedStage)
	{
		return;
	}

	// 스테이지 정보 표시
	if (StageNameText)
	{
		StageNameText->SetText(FText::FromString(SelectedStage->StageName));
	}

	if (StageLevelText)
	{
		StageLevelText->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), SelectedStage->StageLevel)));
	}

	if (StageDescriptionText)
	{
		StageDescriptionText->SetText(SelectedStage->Description);
	}

	UE_LOG(LogTemp, Log, TEXT("[BattleMenuWidget] Stage info updated: %s"), *SelectedStage->StageName);
}

void UBattleMenuWidget::OnStageButtonClicked(UStageData* StageData)
{
	UE_LOG(LogTemp, Log, TEXT("[BattleMenuWidget] OnStageButtonClicked: %s"), *StageData->StageName);
	SelectStage(StageData);
}

void UBattleMenuWidget::StartBattle()
{
	if (!LobbyManager || !SelectedStage)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BattleMenuWidget] Cannot start battle - missing LobbyManager or SelectedStage"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[BattleMenuWidget] Starting battle: %s"), *SelectedStage->StageName);
	LobbyManager->StartBattle(SelectedStage);
}

void UBattleMenuWidget::OnStartBattleButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("[BattleMenuWidget] Start Battle button clicked"));
	StartBattle();
}
