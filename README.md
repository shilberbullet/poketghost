# 포켓고스트 (PoketGhost)

한국 전통 요괴를 소재로 한 턴제 RPG 게임입니다.

## 게임 소개

포켓고스트는 조선시대를 배경으로, 다양한 한국 전통 요괴들을 수집하고 육성하며 전투를 벌이는 RPG 게임입니다. 도깨비, 구미호, 귀신 등 한국의 전통적인 요괴들이 등장하며, 각 요괴는 고유한 특성과 기술을 가지고 있습니다.

## 주요 기능

- **요괴 수집**: 다양한 한국 전통 요괴를 수집하고 육성
- **전투 시스템**: 턴제 전투 시스템으로 요괴들과 전투
- **아이템 시스템**: 부적, 회복 아이템 등 다양한 아이템 사용
- **스테이지 진행**: 다양한 지역과 지형에서 진행되는 스테이지
- **저장 시스템**: 게임 진행 상황 저장 및 불러오기

## 기술 스택

- C 언어
- Windows 콘솔 환경

## 실행 방법

1. 소스 코드 다운로드
```bash
git clone https://github.com/shilberbullet/poketghost.git
```

2. 컴파일
```bash
gcc -o poketghost src/main.c src/menu.c src/game.c src/input.c src/settings.c src/text.c src/stage.c src/yokai.c src/move.c src/battle.c src/party.c src/savefile.c src/item.c -Wall -Wextra
```

3. 실행
```bash
./poketghost
```

## 게임 조작

- 숫자 키를 사용하여 메뉴 선택
- 전투 시 기술 선택 및 아이템 사용
- 저장 및 불러오기 기능
- 메인 메뉴에서 '3'을 눌러 아이템 목록 확인 가능
- 전투 중 '2'를 눌러 부적 사용 가능

## 프로젝트 구조

```
poketghost/
├── src/           # 소스 코드
├── data/          # 게임 데이터 파일
│   ├── move.txt   # 기술 데이터
│   └── yokai.txt  # 요괴 데이터
└── README.md      # 프로젝트 설명
```

## 개발자

- shilberbullet

## 라이선스

이 프로젝트는 MIT 라이선스를 따릅니다. 