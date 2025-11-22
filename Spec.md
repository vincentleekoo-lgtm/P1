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

## 다음 작업 예정
- [ ] CharacterBase C++ 클래스 생성
- [ ] PaperFlipbook 컴포넌트 통합
- [ ] PartyMember 클래스 구현
- [ ] Enemy 클래스 구현
- [ ] 애니메이션 스테이트 머신 구현
