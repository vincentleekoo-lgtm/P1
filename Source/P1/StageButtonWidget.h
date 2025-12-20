// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageButtonWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UStageData;

/**
 * 미니맵 위 스테이지 선택 버튼
 */
UCLASS()
class P1_API UStageButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 스테이지 데이터 설정
	UFUNCTION(BlueprintCallable, Category = "Stage Button")
	void SetStageData(UStageData* InStageData);

	// 클릭 이벤트 델리게이트
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStageButtonClicked, UStageData*, StageData);
	
	UPROPERTY(BlueprintAssignable, Category = "Stage Button")
	FOnStageButtonClicked OnStageButtonClickedEvent;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonClicked();

	void UpdateVisuals();

public:
	// ========== UI 요소들 ==========
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StageButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> StageIconImage;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> StageNumberText;

protected:
	UPROPERTY()
	TObjectPtr<UStageData> StageData;
};
