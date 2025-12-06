// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterCardWidget.h"
#include "CharacterData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"

void UCharacterCardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 클릭 이벤트 바인딩
	if (CardButton)
	{
		CardButton->OnClicked.AddDynamic(this, &UCharacterCardWidget::OnCardClicked);
	}
}

void UCharacterCardWidget::SetCharacterData(UCharacterData* InCharacterData, int32 InOwnedCount)
{
	CharacterData = InCharacterData;
	OwnedCount = InOwnedCount;

	if (!CharacterData)
		return;

	// 캐릭터 이름
	if (CharacterNameText)
	{
		CharacterNameText->SetText(FText::FromString(CharacterData->CharacterName));
	}

	// 등급
	if (RarityText)
	{
		FString RarityString;
		switch (CharacterData->Rarity)
		{
		case ECharacterRarity::Common:
			RarityString = TEXT("Common");
			break;
		case ECharacterRarity::Rare:
			RarityString = TEXT("Rare");
			break;
		case ECharacterRarity::SuperRare:
			RarityString = TEXT("Super Rare");
			break;
		case ECharacterRarity::Epic:
			RarityString = TEXT("Epic");
			break;
		case ECharacterRarity::Legendary:
			RarityString = TEXT("Legendary");
			break;
		}
		RarityText->SetText(FText::FromString(RarityString));
	}

	// 소유 개수
	if (OwnedCountText)
	{
		OwnedCountText->SetText(FText::FromString(FString::Printf(TEXT("x%d"), OwnedCount)));
	}

	// 아이콘
	if (CharacterIcon && CharacterData->Icon)
	{
		CharacterIcon->SetBrushFromTexture(CharacterData->Icon);
	}
}

void UCharacterCardWidget::SetSelected(bool bInSelected)
{
	bIsSelected = bInSelected;

	// TODO: 선택 상태에 따른 비주얼 변경
	// 예: 테두리 색상, 배경색 변경
}

void UCharacterCardWidget::OnCardClicked()
{
	// 이벤트 브로드캐스트
	OnCardClickedEvent.Broadcast(this);

	UE_LOG(LogTemp, Log, TEXT("Character card clicked: %s"), 
		CharacterData ? *CharacterData->CharacterName : TEXT("None"));
}

FReply UCharacterCardWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 왼쪽 마우스 버튼으로 드래그 시작 감지
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return FReply::Unhandled();
}

void UCharacterCardWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (!CharacterData)
		return;

	// DragDropOperation 생성
	UDragDropOperation* DragOperation = NewObject<UDragDropOperation>();
	DragOperation->Payload = CharacterData;
	DragOperation->DefaultDragVisual = this;
	DragOperation->Pivot = EDragPivot::MouseDown;

	OutOperation = DragOperation;

	UE_LOG(LogTemp, Log, TEXT("Drag started for: %s"), *CharacterData->CharacterName);
}
