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
- [ ] GameManager 구현
- [ ] StageManager 구현
- [ ] BattleManager 구현
