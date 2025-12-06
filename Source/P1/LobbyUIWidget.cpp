// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyUIWidget.h"
#include "LobbyManager.h"
#include "CharacterData.h"
#include "CharacterCardWidget.h"
#include "DeckSlotWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void ULobbyUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 전투 시작 버튼 바인딩
	if (StartBattleButton)
	{
		StartBattleButton->OnClicked.AddDynamic(this, &ULobbyUIWidget::OnStartBattleClicked);
	}

	// 덱 슬롯 배열 초기화
	DeckSlots.Empty();
	if (DeckSlot_0)
	{
		DeckSlot_0->SetSlotIndex(0);
		DeckSlot_0->OnSlotClickedEvent.AddDynamic(this, &ULobbyUIWidget::OnDeckSlotClicked);
		DeckSlots.Add(DeckSlot_0);
	}
	if (DeckSlot_1)
	{
		DeckSlot_1->SetSlotIndex(1);
		DeckSlot_1->OnSlotClickedEvent.AddDynamic(this, &ULobbyUIWidget::OnDeckSlotClicked);
		DeckSlots.Add(DeckSlot_1);
	}
	if (DeckSlot_2)
	{
		DeckSlot_2->SetSlotIndex(2);
		DeckSlot_2->OnSlotClickedEvent.AddDynamic(this, &ULobbyUIWidget::OnDeckSlotClicked);
		DeckSlots.Add(DeckSlot_2);
	}
	if (DeckSlot_3)
	{
		DeckSlot_3->SetSlotIndex(3);
		DeckSlot_3->OnSlotClickedEvent.AddDynamic(this, &ULobbyUIWidget::OnDeckSlotClicked);
		DeckSlots.Add(DeckSlot_3);
	}
	if (DeckSlot_4)
	{
		DeckSlot_4->SetSlotIndex(4);
		DeckSlot_4->OnSlotClickedEvent.AddDynamic(this, &ULobbyUIWidget::OnDeckSlotClicked);
		DeckSlots.Add(DeckSlot_4);
	}
}

void ULobbyUIWidget::SetLobbyManager(ALobbyManager* InLobbyManager)
{
	LobbyManager = InLobbyManager;
	
	if (LobbyManager)
	{
		InitializeUI();
	}
}

void ULobbyUIWidget::InitializeUI()
{
	if (!LobbyManager)
	{
		UE_LOG(LogTemp, Error, TEXT("LobbyManager is null!"));
		return;
	}

	RefreshCharacterList();
	RefreshDeckSlots();

	UE_LOG(LogTemp, Log, TEXT("Lobby UI Initialized"));
}

void ULobbyUIWidget::RefreshCharacterList()
{
	if (!LobbyManager || !CharacterListScrollBox || !CharacterCardWidgetClass)
		return;

	// 기존 카드 제거
	CharacterListScrollBox->ClearChildren();
	CharacterCards.Empty();

	// 소유한 캐릭터 목록 가져오기
	const TMap<TObjectPtr<UCharacterData>, int32>& OwnedCharacters = LobbyManager->PlayerDeck.OwnedCharacters;

	for (const auto& Pair : OwnedCharacters)
	{
		UCharacterData* CharData = Pair.Key;
		int32 Count = Pair.Value;

		if (!CharData || Count <= 0)
			continue;

		// 캐릭터 카드 위젯 생성
		UCharacterCardWidget* CardWidget = CreateWidget<UCharacterCardWidget>(this, CharacterCardWidgetClass);
		if (CardWidget)
		{
			CardWidget->SetCharacterData(CharData, Count);
			CardWidget->OnCardClickedEvent.AddDynamic(this, &ULobbyUIWidget::OnCharacterCardClicked);
			
			CharacterListScrollBox->AddChild(CardWidget);
			CharacterCards.Add(CardWidget);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Character list refreshed: %d characters"), CharacterCards.Num());
}

void ULobbyUIWidget::RefreshDeckSlots()
{
	if (!LobbyManager)
		return;

	// LobbyManager의 편성 정보 가져와서 슬롯 갱신
	for (int32 i = 0; i < DeckSlots.Num(); i++)
	{
		if (i < LobbyManager->BattleFormation.Num())
		{
			const FBattleSlot& BattleSlotData = LobbyManager->BattleFormation[i];
			
			if (BattleSlotData.bIsActive && BattleSlotData.CharacterData)
			{
				DeckSlots[i]->AssignCharacter(BattleSlotData.CharacterData);
			}
			else
			{
				DeckSlots[i]->ClearSlot();
			}
		}
	}

	// 편성 상태 업데이트
	if (FormationStatusText)
	{
		int32 ActiveCount = 0;
		for (const FBattleSlot& BattleSlotData : LobbyManager->BattleFormation)
		{
			if (BattleSlotData.bIsActive && BattleSlotData.CharacterData)
				ActiveCount++;
		}

		FormationStatusText->SetText(FText::FromString(
			FString::Printf(TEXT("Formation: %d / %d"), ActiveCount, LobbyManager->MaxSlots)
		));
	}
}

void ULobbyUIWidget::OnCharacterCardClicked(UCharacterCardWidget* CardWidget)
{
	if (!CardWidget || !CardWidget->GetCharacterData())
		return;

	SelectedCharacter = CardWidget->GetCharacterData();
	UE_LOG(LogTemp, Log, TEXT("Selected character: %s"), *SelectedCharacter->CharacterName);

	// 선택된 슬롯이 있으면 캐릭터 배치
	if (SelectedSlot)
	{
		if (LobbyManager)
		{
			LobbyManager->AssignCharacterToSlot(SelectedSlot->GetSlotIndex(), SelectedCharacter);
			RefreshDeckSlots();
		}

		SelectedSlot = nullptr;
		SelectedCharacter = nullptr;
	}
}

void ULobbyUIWidget::OnDeckSlotClicked(UDeckSlotWidget* SlotWidget)
{
	if (!SlotWidget)
		return;

	// 슬롯이 비어있으면 선택 대기
	if (SlotWidget->IsEmpty())
	{
		SelectedSlot = SlotWidget;
		UE_LOG(LogTemp, Log, TEXT("Waiting for character selection for slot %d"), SlotWidget->GetSlotIndex());
	}
	// 슬롯에 캐릭터가 있으면 제거
	else
	{
		if (LobbyManager)
		{
			LobbyManager->ClearSlot(SlotWidget->GetSlotIndex());
			RefreshDeckSlots();
		}
	}
}

void ULobbyUIWidget::OnStartBattleClicked()
{
	if (!LobbyManager)
		return;

	if (LobbyManager->IsFormationValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Starting battle!"));
		LobbyManager->StartBattle();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot start battle: Invalid formation"));
		
		// TODO: 경고 메시지 UI 표시
	}
}
