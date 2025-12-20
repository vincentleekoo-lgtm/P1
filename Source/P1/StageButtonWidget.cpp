// Fill out your copyright notice in the Description page of Project Settings.

#include "StageButtonWidget.h"
#include "StageData.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UStageButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StageButton)
	{
		StageButton->OnClicked.AddDynamic(this, &UStageButtonWidget::OnButtonClicked);
	}
}

void UStageButtonWidget::SetStageData(UStageData* InStageData)
{
	StageData = InStageData;
	UpdateVisuals();
}

void UStageButtonWidget::OnButtonClicked()
{
	if (StageData)
	{
		UE_LOG(LogTemp, Log, TEXT("[StageButton] Clicked: %s"), *StageData->StageName);
		OnStageButtonClickedEvent.Broadcast(StageData);
	}
}

void UStageButtonWidget::UpdateVisuals()
{
	if (!StageData)
	{
		return;
	}

	// 스테이지 아이콘 설정
	if (StageIconImage && StageData->StageIcon)
	{
		StageIconImage->SetBrushFromTexture(StageData->StageIcon);
	}

	// 스테이지 번호 표시
	if (StageNumberText)
	{
		StageNumberText->SetText(FText::FromString(FString::Printf(TEXT("%d"), StageData->StageLevel)));
	}

	// 잠금 상태 처리
	if (StageButton)
	{
		StageButton->SetIsEnabled(StageData->bIsUnlocked);
	}

	UE_LOG(LogTemp, Log, TEXT("[StageButton] Updated: %s (Unlocked: %s)"), 
		*StageData->StageName, StageData->bIsUnlocked ? TEXT("Yes") : TEXT("No"));
}
