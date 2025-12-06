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

## 다음 작업 예정
- [ ] Dragon DataAsset 생성
- [ ] 슬롯에서 캐릭터 제거 기능 (우클릭 또는 드래그 아웃)
- [ ] 덱 중복 배치 방지 로직
- [ ] 전투 시작 버튼 활성화 (편성 유효성 검증)
- [ ] SpawnManager 수정 (CharacterData 기반 스폰)
- [ ] BattleManager 수정 (Team 기반 전투 관리)
- [ ] LobbyManager ↔ StageManager 연동
- [ ] 캐릭터 획득 시스템
- [ ] 세이브/로드 시스템
