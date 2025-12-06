// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyManager.h"

ALobbyManager::ALobbyManager()
{
	PrimaryActorTick.bCanEverTick = false;

	MaxSlots = 5;
}

void ALobbyManager::BeginPlay()
{
	Super::BeginPlay();
	
	Initialize();
}

void ALobbyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALobbyManager::Initialize()
{
	UE_LOG(LogTemp, Log, TEXT("LobbyManager Initialized"));
	
	// 슬롯 초기화
	InitializeSlots();
	
	// 테스트용: 기본 캐릭터 추가 (나중에 제거)
	// AddCharacterToDeck(SomeCharacterData, 1);
}

void ALobbyManager::InitializeSlots()
{
	BattleFormation.Empty();
	
	for (int32 i = 0; i < MaxSlots; i++)
	{
		FBattleSlot NewSlot;
		NewSlot.Position = i;
		NewSlot.bIsActive = false;
		NewSlot.CharacterData = nullptr;
		
		BattleFormation.Add(NewSlot);
	}
	
	UE_LOG(LogTemp, Log, TEXT("Battle formation initialized with %d slots"), MaxSlots);
}

// ========== 덱 관리 ==========

void ALobbyManager::AddCharacterToDeck(UCharacterData* Character, int32 Count)
{
	if (!Character || Count <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid character or count"));
		return;
	}

	if (PlayerDeck.OwnedCharacters.Contains(Character))
	{
		PlayerDeck.OwnedCharacters[Character] += Count;
	}
	else
	{
		PlayerDeck.OwnedCharacters.Add(Character, Count);
	}

	UE_LOG(LogTemp, Log, TEXT("Added %d x %s to deck (Total: %d)"), 
		Count, *Character->CharacterName, PlayerDeck.OwnedCharacters[Character]);
}

void ALobbyManager::RemoveCharacterFromDeck(UCharacterData* Character, int32 Count)
{
	if (!Character || Count <= 0)
		return;

	if (!PlayerDeck.OwnedCharacters.Contains(Character))
	{
		UE_LOG(LogTemp, Warning, TEXT("Character not in deck: %s"), *Character->CharacterName);
		return;
	}

	int32& CurrentCount = PlayerDeck.OwnedCharacters[Character];
	CurrentCount -= Count;

	if (CurrentCount <= 0)
	{
		PlayerDeck.OwnedCharacters.Remove(Character);
		UE_LOG(LogTemp, Log, TEXT("Removed all %s from deck"), *Character->CharacterName);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Removed %d x %s from deck (Remaining: %d)"), 
			Count, *Character->CharacterName, CurrentCount);
	}
}

bool ALobbyManager::HasCharacter(UCharacterData* Character) const
{
	if (!Character)
		return false;

	return PlayerDeck.OwnedCharacters.Contains(Character) && 
		   PlayerDeck.OwnedCharacters[Character] > 0;
}

int32 ALobbyManager::GetCharacterCount(UCharacterData* Character) const
{
	if (!Character || !PlayerDeck.OwnedCharacters.Contains(Character))
		return 0;

	return PlayerDeck.OwnedCharacters[Character];
}

// ========== 전투 편성 ==========

bool ALobbyManager::AssignCharacterToSlot(int32 SlotIndex, UCharacterData* Character)
{
	if (SlotIndex < 0 || SlotIndex >= BattleFormation.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid slot index: %d"), SlotIndex);
		return false;
	}

	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot assign null character"));
		return false;
	}

	if (!HasCharacter(Character))
	{
		UE_LOG(LogTemp, Warning, TEXT("Character not owned: %s"), *Character->CharacterName);
		return false;
	}

	// 슬롯에 배치
	BattleFormation[SlotIndex].CharacterData = Character;
	BattleFormation[SlotIndex].bIsActive = true;

	UE_LOG(LogTemp, Log, TEXT("Assigned %s to slot %d"), *Character->CharacterName, SlotIndex);
	return true;
}

void ALobbyManager::ClearSlot(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= BattleFormation.Num())
		return;

	BattleFormation[SlotIndex].CharacterData = nullptr;
	BattleFormation[SlotIndex].bIsActive = false;

	UE_LOG(LogTemp, Log, TEXT("Cleared slot %d"), SlotIndex);
}

void ALobbyManager::ClearAllSlots()
{
	for (FBattleSlot& Slot : BattleFormation)
	{
		Slot.CharacterData = nullptr;
		Slot.bIsActive = false;
	}

	UE_LOG(LogTemp, Log, TEXT("Cleared all battle formation slots"));
}

bool ALobbyManager::IsFormationValid() const
{
	// 최소 1명 이상 배치되어 있어야 함
	for (const FBattleSlot& Slot : BattleFormation)
	{
		if (Slot.bIsActive && Slot.CharacterData != nullptr)
		{
			return true;
		}
	}

	return false;
}

TArray<UCharacterData*> ALobbyManager::GetBattleReadyCharacters() const
{
	TArray<UCharacterData*> ReadyCharacters;

	for (const FBattleSlot& Slot : BattleFormation)
	{
		if (Slot.bIsActive && Slot.CharacterData != nullptr)
		{
			ReadyCharacters.Add(Slot.CharacterData);
		}
	}

	return ReadyCharacters;
}

// ========== 전투 시작 ==========

void ALobbyManager::StartBattle()
{
	if (!IsFormationValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot start battle: Invalid formation"));
		return;
	}

	TArray<UCharacterData*> ReadyCharacters = GetBattleReadyCharacters();
	UE_LOG(LogTemp, Log, TEXT("Starting battle with %d characters"), ReadyCharacters.Num());

	// TODO: StageManager에게 전달
	// StageManager->StartBattleWithFormation(ReadyCharacters);
}
