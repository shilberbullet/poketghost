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

### 1. 개발 환경 준비
1. MinGW 설치 (C 컴파일러)
   - [MinGW 다운로드 페이지](https://sourceforge.net/projects/mingw/)에서 설치
   - 설치 시 `mingw32-base`와 `mingw32-gcc-g++` 패키지 선택
   - 설치 후 시스템 환경 변수 PATH에 MinGW의 bin 폴더 경로 추가
     (예: `C:\MinGW\bin`)

2. Git 설치
   - [Git 다운로드 페이지](https://git-scm.com/downloads)에서 설치

### 2. 프로젝트 다운로드
1. 명령 프롬프트(cmd) 실행
   - Windows 키 + R을 누르고 'cmd' 입력 후 실행

2. 프로젝트 폴더 생성 및 이동
```bash
# 원하는 위치로 이동 (예: C 드라이브)
cd C:\

# 프로젝트 폴더 생성
mkdir poketghostv2

# 생성한 폴더로 이동
cd poketghostv2
```

3. GitHub에서 소스 코드 다운로드
```bash
git clone https://github.com/shilberbullet/poketghost.git .
```

### 3. 컴파일 및 실행
1. 소스 코드 컴파일
```bash
gcc -o poketghost src/main.c src/menu.c src/game.c src/input.c src/settings.c src/text.c src/stage.c src/yokai.c src/move.c src/battle.c src/party.c src/savefile.c src/item.c -I./include -I./src
```

2. 게임 실행
```bash
poketghost
```

## 게임 조작

- 숫자 키를 사용하여 메뉴 선택
- 전투 시 기술 선택 및 아이템 사용
- 저장 및 불러오기 기능

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

- 이상엽

## 최근 업데이트

### 2024-03-XX
- 아이템 보상 시스템 개선
  - 아이템 리스트 초기화 기능 추가 (전 아이템 사용)
  - 초기화 비용 시스템 구현 (100전부터 시작, 스테이지별 2배 증가)
  - 10스테이지마다 초기화 비용 리셋
- 일반 아이템 최대 보유 개수 99개로 변경
  - 작두와 무당방울은 기존 제한 유지
