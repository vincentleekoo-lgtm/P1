// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterCardWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UCharacterData;

/**
 * 캐릭터 카드 UI 위젯
 * - 캐릭터 아이콘, 이름, 등급 표시
 * - 클릭 시 덱에 추가/제거
 */
UCLASS()
class P1_API UCharacterCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 캐릭터 데이터 설정
	UFUNCTION(BlueprintCallable, Category = "Character Card")
	void SetCharacterData(UCharacterData* InCharacterData, int32 InOwnedCount);

	// 선택 상태 설정
	UFUNCTION(BlueprintCallable, Category = "Character Card")
	void SetSelected(bool bInSelected);

	// 캐릭터 데이터 가져오기
	UFUNCTION(BlueprintCallable, Category = "Character Card")
	UCharacterData* GetCharacterData() const { return CharacterData; }

protected:
	virtual void NativeConstruct() override;

	// 드래그 앤 드롭 지원
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	// 버튼 클릭 이벤트
	UFUNCTION()
	void OnCardClicked();

public:
	// UI 요소들 (Blueprint에서 바인딩)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CharacterIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CharacterNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RarityText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> OwnedCountText;

	UPROPERTY(meta = (BindWidget), meta = (BindWidgetOptional))
	TObjectPtr<UButton> CardButton;

	// 카드 클릭 이벤트 (Blueprint에서 구독 가능)
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardClicked, UCharacterCardWidget*, CardWidget);
	
	UPROPERTY(BlueprintAssignable, Category = "Character Card")
	FOnCardClicked OnCardClickedEvent;

protected:
	// 현재 캐릭터 데이터
	UPROPERTY()
	TObjectPtr<UCharacterData> CharacterData;

	// 소유 개수
	UPROPERTY()
	int32 OwnedCount;

	// 선택 여부
	UPROPERTY()
	bool bIsSelected;
};
