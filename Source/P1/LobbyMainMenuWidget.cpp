// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyMainMenuWidget.h"
#include "LobbyManager.h"
#include "LobbyUIWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"

void ULobbyMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UE_LOG(LogTemp, Warning, TEXT("[LobbyMainMenuWidget] NativeConstruct called"));

	// 버튼 이벤트 바인딩
	if (BattleButton)
	{
		BattleButton->OnClicked.AddDynamic(this, &ULobbyMainMenuWidget::OnBattleButtonClicked);
	}

	if (DeckManagementButton)
	{
		DeckManagementButton->OnClicked.AddDynamic(this, &ULobbyMainMenuWidget::OnDeckManagementButtonClicked);
	}

	if (HuntingButton)
	{
		HuntingButton->OnClicked.AddDynamic(this, &ULobbyMainMenuWidget::OnHuntingButtonClicked);
	}

	if (GatheringButton)
	{
		GatheringButton->OnClicked.AddDynamic(this, &ULobbyMainMenuWidget::OnGatheringButtonClicked);
	}

	// 기본 카테고리: 전투
	CurrentCategory = ELobbyMenuCategory::Battle;
}

void ULobbyMainMenuWidget::SetLobbyManager(ALobbyManager* InLobbyManager)
{
	LobbyManager = InLobbyManager;

	if (LobbyManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LobbyMainMenuWidget] LobbyManager is valid"));
		UpdatePlayerInfo();
		
		// 덱 관리 위젯에도 LobbyManager 전달
		if (DeckManagementWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("[LobbyMainMenuWidget] DeckManagementWidget found via BindWidget"));
			DeckManagementWidget->SetLobbyManager(LobbyManager);
		}
		else if (ContentSwitcher)
		{
			UE_LOG(LogTemp, Warning, TEXT("[LobbyMainMenuWidget] DeckManagementWidget not bound, searching in ContentSwitcher..."));
			UE_LOG(LogTemp, Warning, TEXT("[LobbyMainMenuWidget] ContentSwitcher has %d widgets"), ContentSwitcher->GetNumWidgets());
			
			// BindWidget으로 못 찾았으면 ContentSwitcher Slot 1에서 직접 찾기
			UWidget* SlotWidget = ContentSwitcher->GetWidgetAtIndex(1);
			if (SlotWidget)
			{
				UE_LOG(LogTemp, Warning, TEXT("[LobbyMainMenuWidget] Slot 1 widget: %s"), *SlotWidget->GetClass()->GetName());
				ULobbyUIWidget* LobbyUI = Cast<ULobbyUIWidget>(SlotWidget);
				if (LobbyUI)
				{
					UE_LOG(LogTemp, Warning, TEXT("[LobbyMainMenuWidget] Found ULobbyUIWidget in Slot 1, calling SetLobbyManager"));
					LobbyUI->SetLobbyManager(LobbyManager);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("[LobbyMainMenuWidget] Slot 1 widget is not ULobbyUIWidget! (Class: %s)"), *SlotWidget->GetClass()->GetName());
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[LobbyMainMenuWidget] Slot 1 is empty!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[LobbyMainMenuWidget] ContentSwitcher is NULL!"));
		}
		
		UE_LOG(LogTemp, Warning, TEXT("[LobbyMainMenuWidget] SetLobbyManager completed"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[LobbyMainMenuWidget] SetLobbyManager called with NULL LobbyManager!"));
	}
}

void ULobbyMainMenuWidget::SwitchCategory(ELobbyMenuCategory NewCategory)
{
	CurrentCategory = NewCategory;

	if (!ContentSwitcher)
	{
		UE_LOG(LogTemp, Warning, TEXT("ContentSwitcher is null!"));
		return;
	}

	// WidgetSwitcher 인덱스 전환
	int32 SwitcherIndex = static_cast<int32>(CurrentCategory);
	ContentSwitcher->SetActiveWidgetIndex(SwitcherIndex);

	UE_LOG(LogTemp, Log, TEXT("Switched to category: %d"), SwitcherIndex);
}

void ULobbyMainMenuWidget::OnBattleButtonClicked()
{
	SwitchCategory(ELobbyMenuCategory::Battle);
	UE_LOG(LogTemp, Log, TEXT("Battle category selected"));
}

void ULobbyMainMenuWidget::OnDeckManagementButtonClicked()
{
	SwitchCategory(ELobbyMenuCategory::DeckManagement);
	UE_LOG(LogTemp, Log, TEXT("Deck Management category selected"));
}

void ULobbyMainMenuWidget::OnHuntingButtonClicked()
{
	SwitchCategory(ELobbyMenuCategory::Hunting);
	UE_LOG(LogTemp, Log, TEXT("Hunting category selected"));
}

void ULobbyMainMenuWidget::OnGatheringButtonClicked()
{
	SwitchCategory(ELobbyMenuCategory::Gathering);
	UE_LOG(LogTemp, Log, TEXT("Gathering category selected"));
}

void ULobbyMainMenuWidget::UpdatePlayerInfo()
{
	if (!LobbyManager)
		return;

	// 플레이어 이름 (향후 PlayerState에서 가져오기)
	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString(TEXT("Player")));
	}

	// 골드
	if (GoldAmountText)
	{
		GoldAmountText->SetText(FText::AsNumber(LobbyManager->Gold));
	}

	// 스태미나
	if (StaminaText)
	{
		FString StaminaString = FString::Printf(TEXT("%d/%d"), LobbyManager->Stamina, LobbyManager->MaxStamina);
		StaminaText->SetText(FText::FromString(StaminaString));
	}
}
