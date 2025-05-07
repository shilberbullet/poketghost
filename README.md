# 포켓 요괴 (Pocket Ghost)

C 언어로 개발된 텍스트 기반 RPG 게임입니다.

## 주요 기능

- 다양한 지역과 지형에서의 모험
- 동료 요괴 시스템
- 전투 시스템
- 보스 요괴 전투
- 게임 속도 조절
- 게임 저장/불러오기

## 개발 환경

- Windows 10
- GCC 컴파일러
- UTF-8 인코딩 지원

## 빌드 방법

```bash
gcc game_start.c poketgost.c team_code.c game_db.c battle_system.c boss_monsters.c -o game
```

## 실행 방법

```bash
./game
```

## 게임 조작

1. 메인 메뉴에서 원하는 옵션 선택
2. 전투 시 동료 요괴와 기술 선택
3. 게임 설정에서 속도 조절 가능

## 파일 구조

- `game_start.c`: 게임 시작 및 메인 메뉴
- `poketgost.c`: 게임 메인 로직
- `team_code.c`: 팀 코드 관련 기능
- `game_db.c`: 게임 데이터 저장/불러오기
- `battle_system.c`: 전투 시스템
- `boss_monsters.c`: 보스 요괴 관련 기능
- `game_db.h`: 데이터 구조체 정의
- `boss_monsters.h`: 보스 요괴 구조체 정의
- `battle_system.h`: 전투 시스템 함수 선언

## 라이선스

MIT License 