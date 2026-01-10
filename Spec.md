# P1 프로젝트 사양서

## 프로젝트 개요
- **프로젝트명**: P1
- **엔진**: Unreal Engine 5.5
- **개발 시작일**: 2025-11-22
- **장르**: 2D 액션 게임

---

## 개발 일지

### 2025-11-22
#### Git 저장소 설정
- Git 저장소 초기화 완료
- `.gitignore` 파일 생성 (Unreal Engine 전용)
- 초기 커밋 완료

#### 캐릭터 시스템 설계
**CharacterBase 클래스**
- 2D 플립북 기반 캐릭터 베이스 클래스
- 애니메이션 플립북:
  - `Idle`: 대기 상태
  - `Walk`: 이동 상태
  - `Slash`: 공격 상태
  - `Dead`: 사망 상태

**상속 구조**
```
CharacterBase (베이스 클래스)
├── PartyMember (아군)
│   └── 플레이어가 조종하거나 동료로 사용하는 캐릭터들
└── Enemy (적군)
    └── 적 캐릭터들
```

**설계 의도**
- CharacterBase에서 공통 플립북 애니메이션 시스템 구현
- PartyMember와 Enemy는 CharacterBase를 상속받아 각각의 고유 로직 추가
- 확장 가능한 구조로 다양한 캐릭터 타입 추가 가능

---

## 기술 스택
- **언어**: C++ (Unreal Engine)
- **버전 관리**: Git
- **에셋**: Fantasy Heroes (Bowman, Cleric, Magician, Swordsman, Thief, Warrior)

---

---

## 게임 구조 설계

### 2025-11-29
#### 전체 아키텍처
```
GameManager (GameInstance Subsystem)
  - 게임 전체 상태 관리
  - 로그인/아웃게임 관리
  - 컷신 재생
  - 스테이지 전환
  
  └── StageManager (Actor/GameMode)
      - 현재 스테이지 정보
      - 웨이브/목표 관리
      
      ├── SpawnManager (Actor)
      │   - 캐릭터 초기 배치
      │   - 스폰 위치 관리
      
      └── BattleManager (Actor)
          - 전투 흐름 제어
          - 턴/실시간 전투 관리
          - 승패 판정
```

#### 게임 상태(State) 정의
- `Login`: 로그인 화면
- `OutGame`: 메인 메뉴, 캐릭터 선택, 장비 관리
- `Cutscene`: 스토리 컷신 재생
- `InBattle`: 전투 진행 중
- `BattleResult`: 전투 결과 화면
- `GameOver`: 게임 오버

#### 게임 플로우
```
Login → OutGame → Cutscene(선택) → InBattle → BattleResult → OutGame
                      ↑                                          ↓
                      └──────────── (다음 스테이지) ────────────┘
```

---

## 다음 작업 예정
- [x] CharacterBase C++ 클래스 생성
- [x] PaperFlipbook 컴포넌트 통합
- [x] PartyMember 클래스 구현
- [x] Enemy 클래스 구현
- [x] SpawnManager 구현
- [x] GameManager 구현
- [x] StageManager 구현
- [x] BattleManager 구현
- [x] 전투 시스템 구현 (체력, 공격, 사망 처리)
- [x] 승리/패배 조건 구현

---

## 아키텍처 리팩토링 (2025-12-06)

### 덱 시스템 전환
기존 PartyMember/Enemy 고정 구조에서 **덱 기반 시스템**으로 전환

#### 새로운 설계 철학
> "같은 캐릭터를 아군으로도, 적으로도 사용 가능"

#### CharacterData DataAsset 시스템
```
UCharacterData (PrimaryDataAsset)
├── CharacterName: FString
├── Description: FText
├── Rarity: ECharacterRarity
│   ├── Common
│   ├── Rare
│   ├── SuperRare
│   ├── Epic
│   └── Legendary
├── BaseStats: FCharacterStats
│   ├── MaxHealth: int32
│   ├── AttackPower: int32
│   ├── Defense: int32
│   ├── AttackSpeed: float
│   └── MoveSpeed: float
├── Animations: FCharacterAnimations
│   ├── IdleFlipbook
│   ├── WalkFlipbook
│   ├── SlashFlipbook
│   └── DeadFlipbook
└── Icon: UTexture2D (UI용)
```

#### 리팩토링된 CharacterBase
```cpp
CharacterBase (Actor)
├── CharacterData: UCharacterData*  // DataAsset 참조
├── Team: ETeam                     // Ally/Enemy/Neutral
├── CurrentHealth, MaxHealth        // 전투 중 변동 스탯
├── AttackPower, AttackInterval
└── InitializeFromData()            // DataAsset에서 로드
```

**핵심 변경사항:**
- 플립북을 개별 속성에서 CharacterData로 통합
- Team 속성으로 아군/적군 동적 할당
- DataAsset 재사용으로 같은 캐릭터 양쪽 팀 사용 가능

---

## 로비 시스템 (2025-12-06)

### LobbyManager
플레이어 덱 관리 및 전투 편성 시스템

```cpp
LobbyManager (Actor)
├── PlayerDeck: FPlayerDeck
│   └── OwnedCharacters: TMap<UCharacterData*, int32>
│       // 소유한 캐릭터와 개수
├── BattleFormation: TArray<FBattleSlot>  // 최대 5개
│   └── FBattleSlot
│       ├── CharacterData: UCharacterData*
│       ├── Position: int32 (0-4)
│       └── bIsActive: bool
└── Functions
    ├── AddCharacterToDeck()
    ├── AssignCharacterToSlot()
    ├── IsFormationValid()
    └── StartBattle()
```

### UI 시스템 (UMG)

#### CharacterCardWidget
캐릭터 카드 UI (소유 캐릭터 목록)
- CharacterIcon: 캐릭터 아이콘
- CharacterNameText: 이름
- RarityText: 등급 (Common/Rare/SuperRare...)
- OwnedCountText: 소유 개수 (x3 등)
- CardButton: 클릭 시 선택

#### DeckSlotWidget  
전투 편성 슬롯 UI (5개)
- SlotBackground: 슬롯 배경
- CharacterIcon: 배치된 캐릭터 아이콘
- SlotNumberText: 슬롯 번호 (1-5)
- SlotButton: 클릭 시 배치/제거

#### LobbyUIWidget
로비 메인 화면
- CharacterListScrollBox: 소유 캐릭터 스크롤 목록
- DeckSlot_0~4: 전투 편성 슬롯 5개
- FormationStatusText: 편성 상태 (3/5 등)
- StartBattleButton: 전투 시작

**상호작용 플로우:**
1. 캐릭터 카드 클릭 → 캐릭터 선택
2. 빈 슬롯 클릭 → 선택한 캐릭터 배치
3. 채워진 슬롯 클릭 → 캐릭터 제거
4. 전투 시작 버튼 → IsFormationValid() 체크 후 전투 시작

---

## 기술 스택 업데이트
- **언어**: C++ (Unreal Engine)
- **UI**: UMG (Unreal Motion Graphics)
- **모듈**: Core, CoreUObject, Engine, Paper2D, UMG, Slate, SlateCore
- **버전 관리**: Git + GitHub
- **에셋**: Fantasy Heroes, CharacterData DataAssets

---

## DataTable CSV 임포트 시스템 (2025-12-06)

### CharacterDataTableRow
CSV 파일로 캐릭터 데이터 일괄 정의

**파일 위치:** `Content/Data/CharacterDataTable.csv`

**CSV 구조:**
```csv
---,CharacterName,RarityType,MaxHealth,AttackPower,Defense,AttackSpeed,MoveSpeed,Description
Warrior,Warrior,Common,100,15,5,1.0,300,기본 전사
Orc,Orc,Common,80,12,3,1.2,280,오크 전사
Knight,Knight,Rare,120,18,8,0.9,250,중장갑 기사
Archer,Archer,Rare,70,20,2,1.5,320,원거리 궁수
Mage,Mage,SuperRare,60,25,1,1.8,300,마법사
Dragon,Dragon,Legendary,200,30,10,0.8,200,전설 드래곤
```

### DataAsset 자동 임포트 기능
CharacterData에서 DataTable Row 선택 시 스탯 자동 채움

**에디터 워크플로우:**
1. Data Asset 생성 (DA_Warrior)
2. **Data Import** 섹션에서 **DataTableRowName** 드롭다운
3. "Warrior" 선택 → 스탯 자동 입력
4. **Animations** 섹션에서 Flipbook만 수동 할당
5. 저장

**자동 채워지는 항목:**
- Character Name, Rarity, Description
- MaxHealth, AttackPower, Defense, AttackSpeed, MoveSpeed

**수동 할당 항목:**
- IdleFlipbook, WalkFlipbook, SlashFlipbook, DeadFlipbook
- Icon (UI용 Texture2D)

---

## 드래그 앤 드롭 시스템 (2025-12-06)

### CharacterCardWidget 드래그 지원
- `NativeOnMouseButtonDown()`: 드래그 시작 감지
- `NativeOnDragDetected()`: DragDropOperation 생성
- Payload: CharacterData 전달
- DefaultDragVisual: 카드 자체가 마우스를 따라 이동

### DeckSlotWidget 드롭 지원
- `NativeOnDrop()`: 드롭된 CharacterData 받아서 슬롯에 배치
- `NativeOnDragEnter()`: 드래그 진입 시 녹색 하이라이트
- `NativeOnDragLeave()`: 드래그 이탈 시 원래 색상 복구

**사용자 경험:**
1. 캐릭터 카드를 클릭하고 드래그
2. 슬롯 위로 가져가면 녹색으로 하이라이트
3. 슬롯에 드롭하면 캐릭터 아이콘 표시
4. 자유 배치 (Canvas Panel) 레이아웃

---

## 완료된 작업 (2025-12-06)
- [x] CharacterData DataAsset 시스템 구축
- [x] ECharacterRarity enum (5단계 등급)
- [x] ETeam enum (Ally/Enemy/Neutral)
- [x] CharacterBase 리팩토링 (DataAsset 기반)
- [x] ApplyDamage 함수명 변경 (TakeDamage 충돌 해결)
- [x] LobbyManager 생성 (덱 관리 + 편성 슬롯)
- [x] UMG 모듈 추가 (Build.cs)
- [x] CharacterCardWidget C++ 구현
- [x] DeckSlotWidget C++ 구현
- [x] LobbyUIWidget C++ 구현
- [x] CharacterDataTableRow 구조체 (CSV 파싱)
- [x] CharacterDataTable.csv 생성 (6개 캐릭터)
- [x] DataTable Row 드롭다운 선택 기능
- [x] DataAsset 자동 임포트 (PostEditChangeProperty)
- [x] WBP_CharacterCard Blueprint 생성
- [x] WBP_DeckSlot Blueprint 생성
- [x] WBP_LobbyUI Blueprint 생성 (Canvas Panel 자유 배치)
- [x] CharacterData 애셋 5개 생성 (Warrior, Orc, Knight, Archer, Mage)
- [x] Level Blueprint 설정 (LobbyUI 자동 생성)
- [x] 드래그 앤 드롭 시스템 구현
- [x] LobbyManager UI 프로퍼티 추가 (CardWidgetClass, LobbyUIClass)
- [x] Spec.md 문서 업데이트

---

## 완료된 작업 (2025-12-13)
- [x] ELobbyMenuCategory enum 생성 (Battle, DeckManagement, Hunting, Gathering)
- [x] LobbyMainMenuWidget C++ 클래스 생성
- [x] WBP_LobbyMainMenu Blueprint 생성 (WidgetSwitcher 패턴)
- [x] LobbyManager 플레이어 자원 추가 (Gold, Stamina, MaxStamina)
- [x] 4개 카테고리 버튼 구현 및 전환 로직
- [x] Level Blueprint 수정 (WBP_LobbyMainMenu 사용)
- [x] GitHub 커밋 및 푸시 (commit e616cd8)

---

## 완료된 작업 (2025-12-20)

### Phase 3: 덱 관리 통합
- [x] WBP_LobbyUI를 LobbyMainMenuWidget Slot 1에 통합
- [x] DeckManagementWidget BindWidgetOptional 추가
- [x] ContentSwitcher Slot 검색 로직 구현
- [x] SetLobbyManager 자동 전파 (MainMenu → DeckManagement)
- [x] Level Blueprint 수정 (SetLobbyManager 호출 추가)
- [x] 상세 로그 시스템 추가 (디버깅용)

### 전투 시작 시스템 (Phase 2)
- [x] StageData DataAsset 클래스 생성
  - [x] FStageDataTableRow 구조체 (CSV 임포트)
  - [x] CSV 기반 자동 데이터 할당
  - [x] PostEditChangeProperty 구현
- [x] StageDataTable.csv 생성 (6개 스테이지)
- [x] StageButtonWidget C++ 클래스 생성
  - [x] 스테이지 선택 버튼
  - [x] 클릭 이벤트 델리게이트
- [x] BattleMenuWidget C++ 클래스 생성
  - [x] 월드맵 UI
  - [x] 스테이지 버튼 자동 배치 (3x3 그리드)
  - [x] 스테이지 정보 표시
  - [x] 덱 미리보기
  - [x] 전투 시작 버튼
- [x] LobbyManager 확장
  - [x] AvailableStages 배열
  - [x] StartBattle(UStageData*) 함수
  - [x] ShowLobbyUI/HideLobbyUI 함수
  - [x] StageManager 연동
- [x] StageManager 확장
  - [x] InitializeWithStageData() 함수
  - [x] StageData + PlayerFormation 기반 초기화
  - [x] 기존 SpawnManager, BattleManager 활용
- [x] 입력 모드 전환 (UI ↔ Game)

---

## 다음 작업 예정

### Blueprint 작업 (우선순위)
- [ ] StageDataTable CSV 임포트 (에디터)
- [ ] StageData 애셋 생성 (DA_Stage_1_1, DA_Stage_1_2, ...)
- [ ] WBP_StageButton Blueprint 생성
- [ ] WBP_BattleMenu Blueprint 생성 및 레이아웃
- [ ] WBP_LobbyMainMenu Slot 0에 WBP_BattleMenu 배치
- [ ] LobbyManager에 AvailableStages 설정
- [ ] 월드맵 이미지 준비 및 적용

### 전투 통합 (Phase 2-2)
- [ ] CharacterBase.ApplyCharacterData() 구현
- [ ] SpawnManager CharacterData 기반 스폰 확장
- [ ] StageManager 캐릭터 스폰 로직 구현
- [ ] 카메라 전환 시스템 (Lobby ↔ Battle)
- [ ] 전투 종료 → 로비 복귀 플로우
- [ ] BattleResultWidget 생성

### 향후 확장
- [ ] 사냥 시스템 (Phase 4)
- [ ] 채집 시스템 (Phase 5)
- [ ] 캐릭터 획득 시스템
- [ ] 세이브/로드 시스템

---

## 로비 메인 메뉴 시스템 (2025-12-13)

### 게임 플로우 개선
기존: 플레이 시작 → 즉시 전투  
**신규**: 플레이 시작 → 로비 메인 메뉴 → 활동 선택 → 해당 시스템

### 로비 메인 메뉴 카테고리
```
LobbyMainMenu (메인 허브)
├── 전투 시작 (Battle)
│   └── 덱 편성 확인 → StageManager → 전투 진입
│
├── 덱 관리 (Deck Management)
│   ├── 캐릭터 목록 확인
│   ├── 전투 편성 (5개 슬롯)
│   ├── 캐릭터 강화 (향후 확장)
│   └── 장비 장착 (향후 확장)
│
├── 사냥 (Hunting)
│   ├── 몬스터 토벌 (경험치/골드 획득)
│   ├── 보스 도전
│   ├── 일일 던전
│   └── 이벤트 전투
│
└── 채집 (Gathering)
    ├── 자원 수집 (재료 아이템)
    ├── 광물 채광
    ├── 약초 채집
    └── 낚시/요리 (향후 확장)
```

### 시스템 구조 설계

#### LobbyMainMenuWidget (UMG)
로비 메인 화면 UI

**UI 구성:**
```
Canvas Panel (Root)
├── Vertical Box (Left Panel - 메뉴 버튼들)
│   ├── Button (BattleButton) - "전투 시작"
│   ├── Button (DeckManagementButton) - "덱 관리"
│   ├── Button (HuntingButton) - "사냥"
│   └── Button (GatheringButton) - "채집"
│
├── Panel (ContentPanel) - 선택한 카테고리 내용 표시 영역
│   └── WidgetSwitcher (카테고리별 UI 전환)
│       ├── Slot 0: BattleMenuWidget
│       ├── Slot 1: DeckManagementWidget (기존 LobbyUIWidget 재활용)
│       ├── Slot 2: HuntingMenuWidget
│       └── Slot 3: GatheringMenuWidget
│
└── Panel (TopBar) - 플레이어 정보
    ├── TextBlock (PlayerName)
    ├── TextBlock (GoldAmount)
    ├── TextBlock (Level)
    └── ProgressBar (Experience)
```

#### 카테고리별 위젯 설계

**1. BattleMenuWidget**
전투 시작 준비 화면
- 현재 편성된 덱 미리보기 (5개 슬롯)
- 스테이지 선택 (1-1, 1-2, ...)
- 난이도 선택 (Normal, Hard, Expert)
- "전투 시작" 버튼 → IsFormationValid() 체크 → StageManager 전환

**2. DeckManagementWidget**
기존 LobbyUIWidget 재활용
- CharacterListScrollBox: 보유 캐릭터 목록
- DeckSlot_0~4: 전투 편성 슬롯
- 드래그 앤 드롭 캐릭터 배치
- "편성 저장" 버튼

**3. HuntingMenuWidget**
사냥터 선택 화면
- 사냥터 목록 (Scroll Box)
  - 사냥터 이름, 추천 레벨, 보상 정보
  - 입장 조건 (스태미나 소모 등)
- 선택한 사냥터 상세 정보
  - 출현 몬스터 목록
  - 드롭 아이템 정보
- "입장" 버튼 → BattleManager (사냥 모드)

**4. GatheringMenuWidget**
채집 활동 화면
- 채집 장소 목록
  - 숲, 광산, 호수 등
  - 획득 가능 자원 표시
- 채집 미니게임 (향후 구현)
- 획득한 재료 목록
- "채집 시작" 버튼

### 데이터 구조

#### ELobbyMenuCategory (Enum)
```cpp
UENUM(BlueprintType)
enum class ELobbyMenuCategory : uint8
{
    Battle UMETA(DisplayName = "Battle"),
    DeckManagement UMETA(DisplayName = "Deck Management"),
    Hunting UMETA(DisplayName = "Hunting"),
    Gathering UMETA(DisplayName = "Gathering")
};
```

#### FHuntingArea (Struct)
```cpp
USTRUCT(BlueprintType)
struct FHuntingArea
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AreaName;  // "고블린 숲", "오크 요새"

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RecommendedLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StaminaCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UCharacterData*> EnemyList;  // 출현 몬스터

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 GoldReward;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ExpReward;
};
```

#### FGatheringSpot (Struct)
```cpp
USTRUCT(BlueprintType)
struct FGatheringSpot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SpotName;  // "신비로운 숲", "철광산"

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AvailableResources;  // "나무", "철광석"

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StaminaCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float GatheringTime;  // 채집 소요 시간 (초)
};
```

### LobbyManager 확장

```cpp
class ALobbyManager : public AActor
{
    // 기존 덱 관리 기능 유지...

    // 신규: 메뉴 카테고리 관리
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
    ELobbyMenuCategory CurrentCategory;

    // 신규: 사냥터 데이터
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunting")
    TArray<FHuntingArea> HuntingAreas;

    // 신규: 채집 장소 데이터
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gathering")
    TArray<FGatheringSpot> GatheringSpots;

    // 신규: 플레이어 자원
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
    int32 Gold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
    int32 Stamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
    int32 MaxStamina;

    // 신규: 카테고리 전환
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void SwitchCategory(ELobbyMenuCategory NewCategory);

    // 신규: 사냥 시작
    UFUNCTION(BlueprintCallable, Category = "Hunting")
    void StartHunting(FHuntingArea HuntingArea);

    // 신규: 채집 시작
    UFUNCTION(BlueprintCallable, Category = "Gathering")
    void StartGathering(FGatheringSpot GatheringSpot);
};
```

### 구현 우선순위

**Phase 1: 메인 메뉴 UI 구조 (현재 작업)**
- [ ] ELobbyMenuCategory enum 생성
- [ ] LobbyMainMenuWidget C++ 클래스 생성
- [ ] WBP_LobbyMainMenu Blueprint 생성
- [ ] 4개 카테고리 버튼 배치
- [ ] WidgetSwitcher로 카테고리 전환

**Phase 2: 전투 시작 플로우**
- [ ] StageData DataAsset 생성 (스테이지 정보)
- [ ] BattleMenuWidget 생성
  - [ ] 미니맵 UI (월드맵 이미지)
  - [ ] 스테이지 버튼들 (미니맵 위 배치)
  - [ ] 덱 미리보기 패널 (현재 편성 표시)
  - [ ] 전투 시작 버튼
- [ ] 스테이지 선택 → 전투 시작 플로우
  - [ ] StageManager 생성 및 초기화
  - [ ] 카메라 전환 (Lobby → Battle)
  - [ ] BattleManager 전투 시작

**Phase 3: 덱 관리 통합**
- [ ] 기존 LobbyUIWidget → DeckManagementWidget 리팩토링
- [ ] 메인 메뉴에서 덱 관리 진입
- [ ] 편성 저장 기능

**Phase 4: 사냥 시스템 (확장)**
- [ ] FHuntingArea struct 구현
- [ ] HuntingMenuWidget 생성
- [ ] 사냥터 목록 표시
- [ ] 사냥 입장 → BattleManager (사냥 모드)

**Phase 5: 채집 시스템 (확장)**
- [ ] FGatheringSpot struct 구현
- [ ] GatheringMenuWidget 생성
- [ ] 채집 장소 목록
- [ ] 채집 미니게임 (단순 타이머)

---

## 전투 시작 시스템 설계 (2025-12-20)

### 시스템 개요
사용자가 "전투 시작" 카테고리 선택 → 미니맵에서 스테이지 선택 → 전투 시작

### StageData (DataAsset)
각 스테이지의 정보를 저장하는 DataAsset

```cpp
UCLASS(BlueprintType)
class UStageData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    // 스테이지 기본 정보
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage Info")
    FString StageName;  // "1-1 숲의 입구"

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage Info")
    int32 StageLevel;  // 권장 레벨

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage Info")
    FText Description;  // "오크들이 숲을 점령했다!"

    // 미니맵 위치 (UI상 버튼 위치)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
    FVector2D MapPosition;  // (X, Y) 좌표

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
    UTexture2D* StageIcon;  // 미니맵 아이콘

    // 전투 설정
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle")
    TArray<TObjectPtr<UCharacterData>> EnemyCharacters;  // 등장 적

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle")
    int32 EnemyCount;  // 적 수

    // 보상
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rewards")
    int32 GoldReward;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rewards")
    int32 ExpReward;

    // 해금 조건
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlock")
    bool bIsUnlocked = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlock")
    TObjectPtr<UStageData> RequiredPreviousStage;  // 이전 스테이지 클리어 필요
};
```

### BattleMenuWidget (C++)
전투 시작 UI - 미니맵 + 스테이지 선택 + 전투 시작

```cpp
UCLASS()
class UBattleMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // LobbyManager 설정
    UFUNCTION(BlueprintCallable, Category = "Battle Menu")
    void SetLobbyManager(ALobbyManager* InLobbyManager);

    // 스테이지 선택
    UFUNCTION(BlueprintCallable, Category = "Battle Menu")
    void SelectStage(UStageData* Stage);

    // 전투 시작
    UFUNCTION(BlueprintCallable, Category = "Battle Menu")
    void StartBattle();

protected:
    virtual void NativeConstruct() override;

    // UI 초기화
    void InitializeStageButtons();
    void UpdateDeckPreview();
    void UpdateStageInfo();

    // 버튼 이벤트
    UFUNCTION()
    void OnStartBattleButtonClicked();

public:
    // ========== UI 요소들 ==========

    // 미니맵
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> MinimapImage;

    // 스테이지 버튼 컨테이너 (Canvas Panel)
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UCanvasPanel> StageButtonContainer;

    // 선택된 스테이지 정보 패널
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> StageNameText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> StageLevelText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> StageDescriptionText;

    // 덱 미리보기 (현재 편성된 캐릭터들)
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UHorizontalBox> DeckPreviewBox;

    // 전투 시작 버튼
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> StartBattleButton;

    // 스테이지 버튼 위젯 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UStageButtonWidget> StageButtonWidgetClass;

protected:
    // LobbyManager 참조
    UPROPERTY()
    TObjectPtr<ALobbyManager> LobbyManager;

    // 선택된 스테이지
    UPROPERTY()
    TObjectPtr<UStageData> SelectedStage;

    // 생성된 스테이지 버튼들
    UPROPERTY()
    TArray<TObjectPtr<class UStageButtonWidget>> StageButtons;
};
```

### StageButtonWidget (C++)
미니맵 위 스테이지 선택 버튼

```cpp
UCLASS()
class UStageButtonWidget : public UUserWidget
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

public:
    // UI 요소
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> StageButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> StageIconImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> StageNumberText;

protected:
    UPROPERTY()
    TObjectPtr<UStageData> StageData;
};
```

### LobbyManager 확장

```cpp
class ALobbyManager : public AActor
{
    // 기존 코드...

    // 스테이지 목록
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
    TArray<TObjectPtr<UStageData>> AvailableStages;

    // 현재 선택된 스테이지
    UPROPERTY(BlueprintReadWrite, Category = "Battle")
    TObjectPtr<UStageData> CurrentSelectedStage;

    // 전투 시작
    UFUNCTION(BlueprintCallable, Category = "Battle")
    void StartBattle(UStageData* Stage);
};
```

### UI 레이아웃 (WBP_BattleMenu)

```
┌─────────────────────────────────────────────────┐
│  전투 시작                                      │
├─────────────────────────────────────────────────┤
│                                                 │
│  ┌─────────────────┐   ┌────────────────────┐ │
│  │                 │   │ 선택된 스테이지     │ │
│  │   미니맵        │   │                    │ │
│  │                 │   │ 이름: 1-1 숲의입구 │ │
│  │  ●  ●  ●       │   │ 레벨: 5            │ │
│  │    ●  ●        │   │                    │ │
│  │  ●     ●       │   │ 설명: ...          │ │
│  │                 │   │                    │ │
│  └─────────────────┘   └────────────────────┘ │
│                                                 │
│  현재 덱 편성                                   │
│  ┌───┬───┬───┬───┬───┐                        │
│  │ 전│ 궁│ 법│   │   │                        │
│  └───┴───┴───┴───┴───┘                        │
│                                                 │
│                      [전투 시작]               │
└─────────────────────────────────────────────────┘
```

### 전투 시작 플로우

1. **사용자가 "전투 시작" 카테고리 선택**
   - LobbyMainMenuWidget → SwitchCategory(Battle)
   - ContentSwitcher → Slot 0 (BattleMenuWidget) 활성화

2. **BattleMenuWidget 초기화**
   - `SetLobbyManager()` 호출
   - `InitializeStageButtons()` → LobbyManager.AvailableStages 읽어서 버튼 생성
   - 각 StageButton을 MinimapImage 위 Canvas Panel에 배치 (StageData.MapPosition 기준)
   - `UpdateDeckPreview()` → 현재 BattleFormation 표시

3. **스테이지 선택**
   - 사용자가 미니맵의 StageButton 클릭
   - `SelectStage(StageData)` 호출
   - 선택된 스테이지 정보 표시 (이름, 레벨, 설명, 보상)

4. **전투 시작 버튼 클릭**
   - `StartBattle()` 호출
   - LobbyManager.StartBattle(SelectedStage) 호출
   - LobbyManager가 StageManager 생성 및 초기화
   - 카메라 전환 (Lobby Camera → Battle Camera)
   - BattleManager 전투 시작

### StageManager 생성 및 초기화

**기존 StageManager와 통합:**
- StageManager는 이미 SpawnManager, BattleManager 참조를 가지고 있음
- StageData를 받아서 적 캐릭터 정보를 SpawnManager에 전달
- 아군은 LobbyManager의 BattleFormation에서 가져옴

```cpp
// StageManager.h 확장
class AStageManager : public AActor
{
    // 기존 코드...
    
    // 신규: StageData 기반 초기화
    UFUNCTION(BlueprintCallable, Category = "Stage")
    void InitializeWithStageData(UStageData* InStageData, const TArray<FBattleSlot>& PlayerFormation);
    
    // 스테이지 데이터
    UPROPERTY(BlueprintReadOnly, Category = "Stage")
    TObjectPtr<UStageData> StageData;
    
    // 플레이어 편성
    UPROPERTY(BlueprintReadOnly, Category = "Stage")
    TArray<FBattleSlot> PlayerFormation;
};
```

```cpp
// StageManager.cpp 구현
void AStageManager::InitializeWithStageData(UStageData* InStageData, const TArray<FBattleSlot>& PlayerFormation)
{
    if (!InStageData)
    {
        UE_LOG(LogTemp, Error, TEXT("StageData is null!"));
        return;
    }
    
    StageData = InStageData;
    this->PlayerFormation = PlayerFormation;
    
    StageName = StageData->StageName;
    StageNumber = InStageData->StageLevel;
    
    UE_LOG(LogTemp, Log, TEXT("StageManager initialized with StageData: %s"), *StageName);
    
    // BattleManager 초기화
    if (BattleManager)
    {
        BattleManager->Initialize();
        BattleManager->OnBattleCompleted.AddDynamic(this, &AStageManager::HandleBattleCompleted);
    }
    
    // SpawnManager에 캐릭터 데이터 전달
    if (SpawnManager)
    {
        SpawnManager->Initialize();
        
        // 아군 스폰 (PlayerFormation 기반)
        SpawnPlayerCharacters(PlayerFormation);
        
        // 적군 스폰 (StageData 기반)
        SpawnEnemyCharacters(StageData);
    }
    
    // 스테이지 자동 시작
    if (bAutoStartOnBeginPlay)
    {
        StartStage();
    }
}

void AStageManager::SpawnPlayerCharacters(const TArray<FBattleSlot>& Formation)
{
    // SpawnManager를 통해 플레이어 캐릭터 스폰
    for (const FBattleSlot& Slot : Formation)
    {
        if (Slot.bIsActive && Slot.CharacterData)
        {
            // CharacterData 기반으로 PartyMember 스폰
            FVector SpawnLocation = CalculateSpawnPosition(Slot.Position, true); // true = 아군
            APartyMember* Member = SpawnCharacterFromData(Slot.CharacterData, SpawnLocation);
            
            if (Member && BattleManager)
            {
                BattleManager->RegisterPartyMember(Member);
            }
        }
    }
}

void AStageManager::SpawnEnemyCharacters(UStageData* InStageData)
{
    // StageData에서 적 정보 읽어서 스폰
    for (int32 i = 0; i < InStageData->EnemyCount; i++)
    {
        // 랜덤하게 EnemyCharacters 배열에서 선택
        if (InStageData->EnemyCharacters.Num() > 0)
        {
            int32 RandomIndex = FMath::RandRange(0, InStageData->EnemyCharacters.Num() - 1);
            UCharacterData* EnemyData = InStageData->EnemyCharacters[RandomIndex];
            
            FVector SpawnLocation = CalculateSpawnPosition(i, false); // false = 적군
            AEnemy* Enemy = SpawnCharacterFromData(EnemyData, SpawnLocation);
            
            if (Enemy && BattleManager)
            {
                BattleManager->RegisterEnemy(Enemy);
            }
        }
    }
}
```

### LobbyManager → StageManager 연동

```cpp
void ALobbyManager::StartBattle(UStageData* Stage)
{
    if (!Stage || !IsFormationValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot start battle: Invalid stage or formation"));
        return;
    }

    // 1. StageManager 찾기 (레벨에 미리 배치되어 있음)
    AStageManager* StageManager = Cast<AStageManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), AStageManager::StaticClass())
    );
    
    if (!StageManager)
    {
        UE_LOG(LogTemp, Error, TEXT("StageManager not found in level!"));
        return;
    }
    
    // 2. StageData와 플레이어 편성 전달
    StageManager->InitializeWithStageData(Stage, BattleFormation);
    
    // 3. UI 숨기기
    if (MainMenuWidget)
    {
        MainMenuWidget->RemoveFromParent();
    }
    
    // 4. 카메라 전환
    SwitchToBattleCamera();
    
    // 5. 입력 모드 변경 (UI → Game)
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Battle started: %s"), *Stage->StageName);
}

void ALobbyManager::SwitchToBattleCamera()
{
    // Battle Camera는 레벨에 미리 배치되어 있거나 태그로 찾음
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        TArray<AActor*> FoundCameras;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("BattleCamera"), FoundCameras);
        
        if (FoundCameras.Num() > 0)
        {
            PC->SetViewTargetWithBlend(FoundCameras[0], 0.5f);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Battle Camera not found! Use tag 'BattleCamera'"));
        }
    }
}
```

### SpawnManager 확장 (CharacterData 기반 스폰)

**기존 문제:** SpawnManager가 하드코딩된 클래스(WarriorClass, OrcClass)만 스폰

**해결:** CharacterData 기반으로 동적 스폰

```cpp
// SpawnManager.h 확장
class ASpawnManager : public AActor
{
    // 기존 코드...
    
    // 신규: CharacterData 기반 캐릭터 스폰
    UFUNCTION(BlueprintCallable, Category = "Spawn")
    APartyMember* SpawnPartyMember(UCharacterData* CharacterData, FVector Location);
    
    UFUNCTION(BlueprintCallable, Category = "Spawn")
    AEnemy* SpawnEnemy(UCharacterData* CharacterData, FVector Location);
    
    // 기본 PartyMember/Enemy 클래스 (CharacterData 적용용)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TSubclassOf<APartyMember> DefaultPartyMemberClass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TSubclassOf<AEnemy> DefaultEnemyClass;
};
```

```cpp
// SpawnManager.cpp 구현
APartyMember* ASpawnManager::SpawnPartyMember(UCharacterData* CharacterData, FVector Location)
{
    if (!CharacterData || !DefaultPartyMemberClass)
    {
        return nullptr;
    }
    
    FActorSpawnParameters SpawnParams;
    APartyMember* Member = GetWorld()->SpawnActor<APartyMember>(
        DefaultPartyMemberClass, Location, FRotator::ZeroRotator, SpawnParams
    );
    
    if (Member)
    {
        Member->ApplyCharacterData(CharacterData);
        
        if (BattleManager)
        {
            BattleManager->RegisterPartyMember(Member);
        }
        
        UE_LOG(LogTemp, Log, TEXT("Spawned PartyMember: %s at %s"), 
            *CharacterData->CharacterName, *Location.ToString());
    }
    
    return Member;
}

AEnemy* ASpawnManager::SpawnEnemy(UCharacterData* CharacterData, FVector Location)
{
    if (!CharacterData || !DefaultEnemyClass)
    {
        return nullptr;
    }
    
    FActorSpawnParameters SpawnParams;
    AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(
        DefaultEnemyClass, Location, FRotator::ZeroRotator, SpawnParams
    );
    
    if (Enemy)
    {
        Enemy->ApplyCharacterData(CharacterData);
        
        if (BattleManager)
        {
            BattleManager->RegisterEnemy(Enemy);
        }
        
        UE_LOG(LogTemp, Log, TEXT("Spawned Enemy: %s at %s"), 
            *CharacterData->CharacterName, *Location.ToString());
    }
    
    return Enemy;
}
```

### CharacterBase 확장 (CharacterData 적용)

```cpp
// CharacterBase.h
class ACharacterBase : public APaperCharacter
{
    // 기존 코드...
    
    // CharacterData 적용
    UFUNCTION(BlueprintCallable, Category = "Character")
    virtual void ApplyCharacterData(UCharacterData* InCharacterData);
    
protected:
    UPROPERTY(BlueprintReadOnly, Category = "Character")
    TObjectPtr<UCharacterData> CharacterData;
};
```

```cpp
// CharacterBase.cpp
void ACharacterBase::ApplyCharacterData(UCharacterData* InCharacterData)
{
    if (!InCharacterData)
    {
        return;
    }
    
    CharacterData = InCharacterData;
    
    // 스탯 적용
    MaxHealth = InCharacterData->MaxHealth;
    CurrentHealth = MaxHealth;
    AttackPower = InCharacterData->AttackPower;
    Defense = InCharacterData->Defense;
    AttackSpeed = InCharacterData->AttackSpeed;
    MoveSpeed = InCharacterData->MoveSpeed;
    
    // 애니메이션 Flipbook 적용
    if (InCharacterData->IdleFlipbook)
    {
        IdleFlipbook = InCharacterData->IdleFlipbook;
    }
    if (InCharacterData->WalkFlipbook)
    {
        WalkFlipbook = InCharacterData->WalkFlipbook;
    }
    if (InCharacterData->SlashFlipbook)
    {
        SlashFlipbook = InCharacterData->SlashFlipbook;
    }
    if (InCharacterData->DeadFlipbook)
    {
        DeadFlipbook = InCharacterData->DeadFlipbook;
    }
    
    // Idle 애니메이션으로 시작
    PlayAnimation(ECharacterState::Idle);
    
    UE_LOG(LogTemp, Log, TEXT("Applied CharacterData: %s (HP: %d, ATK: %d)"), 
        *InCharacterData->CharacterName, MaxHealth, AttackPower);
}
```

### 전투 종료 → 로비 복귀 플로우

```cpp
// StageManager.cpp - 전투 종료 처리
void AStageManager::HandleBattleCompleted(bool bVictory)
{
    if (bVictory)
    {
        CompleteStage();
        ShowBattleResultUI(true); // 승리 UI
    }
    else
    {
        FailStage();
        ShowBattleResultUI(false); // 패배 UI
    }
}

void AStageManager::ShowBattleResultUI(bool bVictory)
{
    // BattleResultWidget 생성 및 표시
    // "로비로 돌아가기" 버튼 클릭 시 ReturnToLobby() 호출
}

void AStageManager::ReturnToLobby()
{
    // 1. 카메라 전환 (Battle → Lobby)
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        TArray<AActor*> FoundCameras;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("LobbyCamera"), FoundCameras);
        
        if (FoundCameras.Num() > 0)
        {
            PC->SetViewTargetWithBlend(FoundCameras[0], 0.5f);
        }
    }
    
    // 2. 스폰된 캐릭터들 제거
    if (BattleManager)
    {
        BattleManager->ClearAllCharacters();
    }
    
    // 3. LobbyManager 찾아서 UI 다시 표시
    ALobbyManager* LobbyManager = Cast<ALobbyManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), ALobbyManager::StaticClass())
    );
    
    if (LobbyManager)
    {
        LobbyManager->ShowLobbyUI();
    }
    
    // 4. 입력 모드 변경 (Game → UI)
    if (PC)
    {
        FInputModeUIOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
    }
}
```

### LobbyManager UI 재표시

```cpp
// LobbyManager.h
class ALobbyManager : public AActor
{
    // 기존 코드...
    
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowLobbyUI();
    
    UFUNCTION(BlueprintCallable, Category = "UI")
    void HideLobbyUI();
};
```

```cpp
// LobbyManager.cpp
void ALobbyManager::ShowLobbyUI()
{
    if (!MainMenuWidget && MainMenuWidgetClass)
    {
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            MainMenuWidget = CreateWidget<ULobbyMainMenuWidget>(PC, MainMenuWidgetClass);
        }
    }
    
    if (MainMenuWidget)
    {
        MainMenuWidget->SetLobbyManager(this);
        MainMenuWidget->AddToViewport();
        
        UE_LOG(LogTemp, Log, TEXT("Lobby UI shown"));
    }
}

void ALobbyManager::HideLobbyUI()
{
    if (MainMenuWidget)
    {
        MainMenuWidget->RemoveFromParent();
        UE_LOG(LogTemp, Log, TEXT("Lobby UI hidden"));
    }
}
```

### 필요한 에셋
- [ ] 미니맵 이미지 (Texture2D)
- [ ] 스테이지 아이콘들 (Texture2D)
- [ ] StageData 애셋들 (DA_Stage_1_1, DA_Stage_1_2, ...)
- [ ] Battle Camera Actor (레벨에 배치)

---

## 게임 플로우 (업데이트)
```
게임 시작
    ↓
Level Blueprint (BeginPlay)
    ↓
LobbyMainMenuWidget 생성 및 표시
    ↓
┌─────────────────────────────┐
│    로비 메인 메뉴           │
│  [전투] [덱] [사냥] [채집]  │
└─────────────────────────────┘
    ↓
사용자 선택
    ├─→ 전투 시작 → BattleMenuWidget → StageManager → 전투
    ├─→ 덱 관리 → DeckManagementWidget → 캐릭터 편성
    ├─→ 사냥 → HuntingMenuWidget → 사냥터 선택 → 전투
    └─→ 채집 → GatheringMenuWidget → 채집 장소 선택 → 채집
```

### 기술 스택 업데이트
- **UI**: UMG (WidgetSwitcher 활용한 멀티 패널)
- **상태 관리**: ELobbyMenuCategory enum
- **데이터 구조**: FHuntingArea, FGatheringSpot structs
- **네비게이션**: 카테고리 버튼 → WidgetSwitcher 인덱스 전환

---

## 완료된 작업 (2026-01-10)

### StageData 시스템 개선
- [x] FStageDataTableRow에 MapPositionX, MapPositionY 필드 추가
- [x] UStageData에 MapPosition (FVector2D) 필드 추가
- [x] StageDataTable.csv 업데이트 (6개 스테이지, MapPosition 포함)
- [x] StageData.cpp에서 MapPosition 자동 임포트 로직 구현
- [x] P1Editor Win64 Development 빌드 완료
- [x] StageDataTable.csv Reimport 성공

**변경 사항:**
- MapPositionX, MapPositionY → FVector2D MapPosition으로 통합
- 월드맵 UI에서 스테이지 버튼 위치 지정 준비 완료
- 6개 스테이지 데이터 정의 (Stage_1_1 ~ Stage_3_1)

**다음 단계:**
- [ ] WBP_StageButton Blueprint 생성 (UI 레이아웃)
- [ ] WBP_BattleMenu Blueprint 생성 (월드맵 + 스테이지 선택)
- [ ] WBP_LobbyMainMenu Slot 0에 WBP_BattleMenu 통합
- [ ] StageData 애셋 생성 (DA_Stage_1_1 등)
- [ ] 월드맵 배경 이미지 준비

---
