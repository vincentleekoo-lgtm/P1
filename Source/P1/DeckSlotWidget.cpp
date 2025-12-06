// Fill out your copyright notice in the Description page of Project Settings.

#include "DeckSlotWidget.h"
#include "CharacterData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UDeckSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 클릭 이벤트 바인딩
	if (SlotButton)
	{
		SlotButton->OnClicked.AddDynamic(this, &UDeckSlotWidget::OnSlotClicked);
	}
}

void UDeckSlotWidget::SetSlotIndex(int32 InSlotIndex)
{
	SlotIndex = InSlotIndex;

	// 슬롯 번호 표시
	if (SlotNumberText)
	{
		SlotNumberText->SetText(FText::FromString(FString::Printf(TEXT("%d"), SlotIndex + 1)));
	}
}

void UDeckSlotWidget::AssignCharacter(UCharacterData* InCharacterData)
{
	if (!InCharacterData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot assign null character to slot %d"), SlotIndex);
		return;
	}

	CharacterData = InCharacterData;

	// 캐릭터 아이콘 표시
	if (CharacterIcon && CharacterData->Icon)
	{
		CharacterIcon->SetBrushFromTexture(CharacterData->Icon);
		CharacterIcon->SetVisibility(ESlateVisibility::Visible);
	}

	UE_LOG(LogTemp, Log, TEXT("Assigned %s to slot %d"), *CharacterData->CharacterName, SlotIndex);
}

void UDeckSlotWidget::ClearSlot()
{
	CharacterData = nullptr;

	// 아이콘 숨기기
	if (CharacterIcon)
	{
		CharacterIcon->SetVisibility(ESlateVisibility::Hidden);
	}

	UE_LOG(LogTemp, Log, TEXT("Cleared slot %d"), SlotIndex);
}

void UDeckSlotWidget::OnSlotClicked()
{
	// 이벤트 브로드캐스트
	OnSlotClickedEvent.Broadcast(this);

	UE_LOG(LogTemp, Log, TEXT("Slot %d clicked"), SlotIndex);
}

bool UDeckSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (!InOperation || !InOperation->Payload)
		return false;

	// Payload에서 CharacterData 추출
	UCharacterData* DroppedCharacter = Cast<UCharacterData>(InOperation->Payload);
	if (DroppedCharacter)
	{
		AssignCharacter(DroppedCharacter);
		
		// 배경색 원래대로 복원
		if (SlotBackground)
		{
			SlotBackground->SetColorAndOpacity(FLinearColor::White);
		}

		UE_LOG(LogTemp, Log, TEXT("Dropped %s on slot %d"), *DroppedCharacter->CharacterName, SlotIndex);
		return true;
	}

	return false;
}

void UDeckSlotWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	// 드래그 중일 때 하이라이트 표시
	if (SlotBackground)
	{
		SlotBackground->SetColorAndOpacity(FLinearColor(0.5f, 1.0f, 0.5f, 1.0f)); // 연한 녹색
	}

	UE_LOG(LogTemp, Log, TEXT("Drag entered slot %d"), SlotIndex);
}

void UDeckSlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	// 하이라이트 제거
	if (SlotBackground)
	{
		SlotBackground->SetColorAndOpacity(FLinearColor::White);
	}

	UE_LOG(LogTemp, Log, TEXT("Drag left slot %d"), SlotIndex);
}
