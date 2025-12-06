// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeckSlotWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UCharacterData;

/**
 * 덱 슬롯 UI 위젯
 * - 전투 편성 슬롯 표시
 * - 캐릭터 배치/제거
 */
UCLASS()
class P1_API UDeckSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 슬롯 인덱스 설정
	UFUNCTION(BlueprintCallable, Category = "Deck Slot")
	void SetSlotIndex(int32 InSlotIndex);

	// 캐릭터 배치
	UFUNCTION(BlueprintCallable, Category = "Deck Slot")
	void AssignCharacter(UCharacterData* InCharacterData);

	// 슬롯 비우기
	UFUNCTION(BlueprintCallable, Category = "Deck Slot")
	void ClearSlot();

	// 슬롯이 비어있는지 확인
	UFUNCTION(BlueprintCallable, Category = "Deck Slot")
	bool IsEmpty() const { return CharacterData == nullptr; }

	// 슬롯 인덱스 가져오기
	UFUNCTION(BlueprintCallable, Category = "Deck Slot")
	int32 GetSlotIndex() const { return SlotIndex; }

	// 배치된 캐릭터 가져오기
	UFUNCTION(BlueprintCallable, Category = "Deck Slot")
	UCharacterData* GetCharacterData() const { return CharacterData; }

protected:
	virtual void NativeConstruct() override;

	// 드래그 앤 드롭 지원
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// 버튼 클릭 이벤트
	UFUNCTION()
	void OnSlotClicked();

public:
	// UI 요소들
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SlotBackground;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CharacterIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SlotNumberText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SlotButton;

	// 슬롯 클릭 이벤트
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, UDeckSlotWidget*, SlotWidget);
	
	UPROPERTY(BlueprintAssignable, Category = "Deck Slot")
	FOnSlotClicked OnSlotClickedEvent;

protected:
	// 슬롯 인덱스 (0-4)
	UPROPERTY()
	int32 SlotIndex;

	// 배치된 캐릭터
	UPROPERTY()
	TObjectPtr<UCharacterData> CharacterData;
};
