// 표준 입출력 함수를 위한 헤더
#include <stdio.h> // 표준 입출력 함수
// 표준 라이브러리 함수를 위한 헤더
#include <stdlib.h> // 표준 라이브러리 함수
// 문자열 처리 함수를 위한 헤더
#include <string.h> // 문자열 처리 함수
// 수학 함수를 위한 헤더
#include <math.h> // 수학 함수
// 윈도우 API를 위한 헤더 (SYSTEMTIME 구조체를 위해 추가)
#include <windows.h> // 윈도우 API
// 요괴 관련 함수와 구조체 정의
#include "yokai.h" // 요괴 헤더
// 기술 관련 함수
#include "move.h" // 기술 헤더
// 로거 관련 함수
#include "logger.h" // 로거 헤더
// 텍스트 출력 관련 함수
#include "text.h" // 텍스트 출력 헤더
// 도감 관련 함수 (도감 초기화를 위해 추가)
#include "encyclopedia.h" // 도감 헤더
// 시간 관련 함수를 위한 헤더
#include <time.h> // 시간 관련 함수
// 문자 타입 함수를 위한 헤더
#include <ctype.h> // 문자 타입 함수

// 요괴 설명 최대 길이 정의
#ifndef YOKAI_DESC_MAX
#define YOKAI_DESC_MAX 256 // 요괴 설명 최대 길이
#endif

// 요괴 목록 배열
Yokai yokaiList[MAX_YOKAI]; // 일반 요괴 목록
int yokaiListCount = 0; // 일반 요괴 수

// 보스 요괴 목록 배열
Yokai bossYokaiList[MAX_BOSS_YOKAI]; // 보스 요괴 목록
int bossYokaiListCount = 0; // 보스 요괴 수

// 패러독스 요괴 최대 수 정의
#define MAX_PARADOX_YOKAI 16 // 패러독스 요괴 최대 수
// 패러독스 요괴 목록 배열
Yokai paradoxYokaiList[MAX_PARADOX_YOKAI]; // 패러독스 요괴 목록
int paradoxYokaiListCount = 0; // 패러독스 요괴 수

// 요괴 이름 배열
const char* yokaiNames[] = {
    "구미호", "도깨비", "오니", "텐구", "카파",
    "누에", "바케네코", "코다마", "츠치노코", "누라리횬"
}; // 게임 내 등장하는 요괴들의 이름을 저장하는 문자열 배열

// 상성 이름 배열
const char* typeNames[] = {
    "악귀", "원귀", "괴수형", "인간형", "동물형"
}; // 요괴 타입(상성) 이름을 저장하는 배열

// 상성 설명 배열
const char* typeDescriptions[] = {
    "악한 기운을 가진 요괴입니다.",
    "원한을 가진 영혼이 변한 요괴입니다.",
    "괴물과 같은 모습을 한 요괴입니다.",
    "인간과 비슷한 모습을 한 요괴입니다.",
    "동물의 모습을 한 요괴입니다."
}; // 각 타입별 설명을 저장하는 배열

const int YOKAI_COUNT = sizeof(yokaiNames) / sizeof(yokaiNames[0]); // 요괴의 총 개수를 계산

// 상성 관계 테이블 (공격자 -> 방어자)
// 1.0: 보통, 2.0: 효과적, 0.5: 비효과적
static const float typeEffectivenessTable[TYPE_COUNT][TYPE_COUNT] = {
    // 악귀 -> [악귀, 원귀, 괴수형, 인간형, 동물형]
    {1.0f, 2.0f, 2.0f, 1.0f, 1.0f},
    // 원귀 -> [악귀, 원귀, 괴수형, 인간형, 동물형]
    {2.0f, 1.0f, 1.0f, 2.0f, 1.0f},
    // 괴수형 -> [악귀, 원귀, 괴수형, 인간형, 동물형]
    {1.0f, 2.0f, 1.0f, 1.0f, 2.0f},
    // 인간형 -> [악귀, 원귀, 괴수형, 인간형, 동물형]
    {2.0f, 1.0f, 2.0f, 1.0f, 1.0f},
    // 동물형 -> [악귀, 원귀, 괴수형, 인간형, 동물형]
    {1.0f, 2.0f, 1.0f, 2.0f, 1.0f}
}; // 공격자 타입과 방어자 타입에 따른 상성 효과를 저장하는 2차원 배열

// 상성 효과를 계산하는 함수
float calculateTypeEffectiveness(YokaiType attackerType, YokaiType defenderType) {
    LOG_FUNCTION_EXECUTION("calculateTypeEffectiveness"); // 함수 실행 로그 기록
    return typeEffectivenessTable[attackerType][defenderType]; // 상성 테이블에서 효과값 반환
}

// 전역 변수로 다음 ID 값 저장
static unsigned long long nextYokaiId = 1; // 요괴의 고유 ID 생성을 위한 전역 카운터

// ID 생성 함수
static unsigned long long generateYokaiId() {
    LOG_FUNCTION_EXECUTION("generateYokaiId"); // 함수 실행 로그 기록
    // 현재 시간을 밀리초 단위로 가져옴
    SYSTEMTIME st;
    GetSystemTime(&st); // 시스템의 현재 시간을 st에 저장
    
    // 시간 기반 ID 생성 (년월일시분초밀리초)
    unsigned long long timeBasedId = 
        (unsigned long long)st.wYear * 10000000000000ULL + // 연도
        (unsigned long long)st.wMonth * 100000000000ULL + // 월
        (unsigned long long)st.wDay * 1000000000ULL + // 일
        (unsigned long long)st.wHour * 10000000ULL + // 시
        (unsigned long long)st.wMinute * 100000ULL + // 분
        (unsigned long long)st.wSecond * 1000ULL + // 초
        (unsigned long long)st.wMilliseconds; // 밀리초
    
    // 전역 카운터와 조합하여 고유성 보장
    return timeBasedId + (nextYokaiId++); // 시간 기반 ID에 카운터를 더해 반환
}

YokaiType parseType(const char* typeStr) {
    LOG_FUNCTION_EXECUTION("parseType"); // 함수 실행 로그 기록
    if (strcmp(typeStr, "EVIL_SPIRIT") == 0) return TYPE_EVIL_SPIRIT; // 문자열이 "EVIL_SPIRIT"이면 해당 타입 반환
    if (strcmp(typeStr, "GHOST") == 0) return TYPE_GHOST; // "GHOST"이면 해당 타입 반환
    if (strcmp(typeStr, "MONSTER") == 0) return TYPE_MONSTER; // "MONSTER"이면 해당 타입 반환
    if (strcmp(typeStr, "HUMAN") == 0) return TYPE_HUMAN; // "HUMAN"이면 해당 타입 반환
    if (strcmp(typeStr, "ANIMAL") == 0) return TYPE_ANIMAL; // "ANIMAL"이면 해당 타입 반환
    return TYPE_EVIL_SPIRIT;  // 일치하는 타입이 없으면 기본값 반환
}

void loadYokaiFromFile(const char* filename) {
    LOG_FUNCTION_EXECUTION("loadYokaiFromFile"); // 함수 실행 로그 기록
    yokaiListCount = 0; // 일반 요괴 수 초기화
    bossYokaiListCount = 0; // 보스 요괴 수 초기화
    paradoxYokaiListCount = 0; // 패러독스 요괴 수 초기화
    FILE* file = fopen(filename, "r"); // 파일 열기
    if (!file) return; // 파일 열기에 실패하면 함수 종료
    char line[32768]; // 한 줄을 저장할 버퍼
    int isBoss = 0; // 현재 보스 요괴 섹션 여부
    int isParadox = 0; // 현재 패러독스 요괴 섹션 여부
    int lineCount = 0; // 파일 내 줄 번호
    while (fgets(line, sizeof(line), file)) { // 파일에서 한 줄씩 읽기
        lineCount++; // 줄 번호 증가
        if (strstr(line, "# 보스 요괴")) { 
            isBoss = 1; // 보스 요괴 섹션 시작
            isParadox = 0; 
            LOG_FUNCTION_EXECUTION("보스 요괴 섹션 시작");
            continue; 
        }
        if (strstr(line, "# 패러독스 요괴")) { 
            isParadox = 1; // 패러독스 요괴 섹션 시작
            isBoss = 0; 
            LOG_FUNCTION_EXECUTION("패러독스 요괴 섹션 시작");
            continue; 
        }
        if (line[0] == '#' || line[0] == '\n') continue; // 주석 또는 빈 줄은 건너뜀
        line[strcspn(line, "\n")] = 0; // 줄 끝 개행 문자 제거
        char* last_comma = strrchr(line, ','); // 마지막 콤마 위치 찾기
        if (!last_comma) continue; // 콤마가 없으면 건너뜀
        char* moves = last_comma + 1; // 마지막 콤마 뒤가 기술 목록
        *last_comma = '\0'; // 마지막 콤마를 문자열 끝으로 만듦
        char* name = strtok(line, ","); // 이름 추출
        char* type = strtok(NULL, ","); // 타입 추출
        char* attack = strtok(NULL, ","); // 공격력 추출
        char* defense = strtok(NULL, ","); // 방어력 추출
        char* stamina = strtok(NULL, ","); // 체력 추출
        char* speed = strtok(NULL, ","); // 속도 추출
        char* desc = strtok(NULL, ""); // 설명 추출(나머지 전체)
        if (name && type && attack && defense && stamina && speed && desc && moves) {
            Yokai* y;
            char logBuffer[512];
            if (isParadox && paradoxYokaiListCount < MAX_PARADOX_YOKAI) {
                y = &paradoxYokaiList[paradoxYokaiListCount++]; // 패러독스 요괴 배열에 추가
                sprintf(logBuffer, "패러독스 요괴 로드: %s (인덱스: %d)", name, paradoxYokaiListCount);
            } else if (!isBoss && yokaiListCount < MAX_YOKAI) {
                y = &yokaiList[yokaiListCount++]; // 일반 요괴 배열에 추가
                sprintf(logBuffer, "일반 요괴 로드: %s (인덱스: %d)", name, yokaiListCount);
            } else if (isBoss && bossYokaiListCount < MAX_BOSS_YOKAI) {
                y = &bossYokaiList[bossYokaiListCount++]; // 보스 요괴 배열에 추가
                sprintf(logBuffer, "보스 요괴 로드: %s (인덱스: %d)", name, bossYokaiListCount);
            } else {
                sprintf(logBuffer, "요괴 로드 실패: %s (배열 가득참)", name); // 배열이 가득 찬 경우
                LOG_FUNCTION_EXECUTION(logBuffer);
                continue;
            }
            LOG_FUNCTION_EXECUTION(logBuffer); // 요괴 로드 로그
            
            strncpy(y->name, name, YOKAI_NAME_MAX - 1); // 이름 복사
            y->name[YOKAI_NAME_MAX - 1] = '\0'; // 널 종료
            y->type = parseType(type); // 타입 파싱
            y->attack = atoi(attack); // 공격력 변환
            y->defense = atoi(defense); // 방어력 변환
            y->stamina = atoi(stamina); // 체력 변환
            y->speed = atoi(speed); // 속도 변환
            memset(y->desc, 0, YOKAI_DESC_MAX); // 설명 버퍼 초기화
            strncpy(y->desc, desc, YOKAI_DESC_MAX - 1); // 설명 복사
            y->desc[YOKAI_DESC_MAX - 1] = '\0'; // 널 종료
            y->learnableMoveCount = 0; // 배울 수 있는 기술 개수 초기화
            y->yokaiInventoryCount = 0;  // 인벤토리 개수 초기화
            char* moveName = strtok(moves, ";"); // 기술 이름 추출
            while (moveName && y->learnableMoveCount < MAX_LEARNABLE_MOVES) {
                Move* m = findMoveByName(moveName); // 기술 이름으로 Move 구조체 찾기
                if (m) {
                    y->learnableMoves[y->learnableMoveCount++] = *m; // 찾은 기술 추가
                } else {
                    // 기술을 찾지 못한 경우 기본 기술 생성
                    char debugBuffer[256];
                    sprintf(debugBuffer, "[DEBUG] 기술을 찾을 수 없음: '%s'", moveName);
                    LOG_FUNCTION_EXECUTION(debugBuffer);
                    
                    Move defaultMove;
                    strncpy(defaultMove.name, moveName, YOKAI_NAME_MAX - 1); // 이름 복사
                    defaultMove.name[YOKAI_NAME_MAX - 1] = '\0'; // 널 종료
                    defaultMove.type = TYPE_EVIL_SPIRIT;  // 기본 타입
                    defaultMove.power = 50;               // 기본 위력
                    defaultMove.accuracy = 90;            // 기본 명중률
                    defaultMove.pp = 10;                  // 기본 PP
                    strncpy(defaultMove.description, "기본 기술", 255);
                    defaultMove.description[255] = '\0';
                    defaultMove.grade = MOVE_BASIC;       // 기본 등급
                    y->learnableMoves[y->learnableMoveCount++] = defaultMove; // 기본 기술 추가
                }
                moveName = strtok(NULL, ";"); // 다음 기술 이름 추출
            }
            // 최소 1개의 기술은 보장
            if (y->learnableMoveCount == 0) {
                Move defaultMove;
                strncpy(defaultMove.name, "기본공격", YOKAI_NAME_MAX - 1); // 이름 복사
                defaultMove.name[YOKAI_NAME_MAX - 1] = '\0'; // 널 종료
                defaultMove.type = TYPE_EVIL_SPIRIT;
                defaultMove.power = 50;
                defaultMove.accuracy = 90;
                defaultMove.pp = 10;
                strncpy(defaultMove.description, "기본 기술", 255);
                defaultMove.description[255] = '\0';
                defaultMove.grade = MOVE_BASIC;
                y->learnableMoves[y->learnableMoveCount++] = defaultMove; // 기본 기술 추가
            }
            y->moveCount = 0; // 실제 장착된 기술 개수 초기화
        }
    }
    fclose(file); // 파일 닫기
    
    // 로드된 요괴 수 로그 출력
    char logBuffer[256];
    sprintf(logBuffer, "로드된 요괴 수: 일반=%d, 보스=%d, 패러독스=%d, 총=%d", 
            yokaiListCount, bossYokaiListCount, paradoxYokaiListCount, 
            yokaiListCount + bossYokaiListCount + paradoxYokaiListCount);
    LOG_FUNCTION_EXECUTION(logBuffer); // 최종 로드 결과 로그

    // 도감 데이터 로드 (저장된 데이터가 있으면 로드, 없으면 도깨비만 초기화)
    loadCaughtYokaiData(); // 이미 잡은 요괴 데이터 로드
}

// learnableMoves에서 랜덤 4개를 moves에 복사하는 함수
void assignRandomMoves(Yokai* y, int level) {
    LOG_FUNCTION_EXECUTION("assignRandomMoves"); // 함수 실행 로그 기록

    // 일반 요괴들의 기존 로직
    Move basicMoves[MAX_LEARNABLE_MOVES]; // 기본 등급 기술 저장 배열
    int basicCount = 0; // 기본 등급 기술 개수
    Move mediumMoves[MAX_LEARNABLE_MOVES]; // 중급 등급 기술 저장 배열
    int mediumCount = 0; // 중급 등급 기술 개수
    Move advancedMoves[MAX_LEARNABLE_MOVES]; // 고급 등급 기술 저장 배열
    int advancedCount = 0; // 고급 등급 기술 개수

    for (int i = 0; i < y->learnableMoveCount; i++) { // 배울 수 있는 모든 기술에 대해
        switch (y->learnableMoves[i].grade) { // 등급에 따라 분류
            case MOVE_BASIC:
                basicMoves[basicCount++] = y->learnableMoves[i]; // 기본 등급 기술 추가
                break;
            case MOVE_MEDIUM:
                mediumMoves[mediumCount++] = y->learnableMoves[i]; // 중급 등급 기술 추가
                break;
            case MOVE_ADVANCED:
                advancedMoves[advancedCount++] = y->learnableMoves[i]; // 고급 등급 기술 추가
                break;
        }
    }

    // 각 등급별로 Fisher-Yates 셔플 적용
    for (int i = basicCount - 1; i > 0; i--) {
        int j = rand() % (i + 1); // 0~i 중 랜덤 인덱스
        Move temp = basicMoves[i]; basicMoves[i] = basicMoves[j]; basicMoves[j] = temp; // 스왑
    }
    for (int i = mediumCount - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Move temp = mediumMoves[i]; mediumMoves[i] = mediumMoves[j]; mediumMoves[j] = temp;
    }
    for (int i = advancedCount - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Move temp = advancedMoves[i]; advancedMoves[i] = advancedMoves[j]; advancedMoves[j] = temp;
    }

    y->moveCount = 0; // 실제 장착 기술 개수 초기화
    int needed = MAX_MOVES; // 필요한 기술 슬롯 수

    int basicToAssign = 0; // 배정할 기본기 개수
    int mediumToAssign = 0; // 배정할 중급기 개수
    int advancedToAssign = 0; // 배정할 고급기 개수

    if (level < 10) {
        basicToAssign = 4; // 10레벨 미만은 기본기 4개
    } else if (level < 30) {
        basicToAssign = 2; // 10~29레벨은 기본기 2개
        mediumToAssign = 2; // 중급기 2개
    } else {
        basicToAssign = 1; // 30레벨 이상은 기본기 1개
        mediumToAssign = 2; // 중급기 2개
        advancedToAssign = 1; // 고급기 1개
    }

    // 계산된 개수만큼 기술을 장착
    int assignedCount = 0;
    for (int i = 0; i < basicToAssign && assignedCount < needed && i < basicCount; i++) {
        y->moves[assignedCount].move = basicMoves[i]; // 기본기 장착
        y->moves[assignedCount].currentPP = basicMoves[i].pp; // PP 설정
        assignedCount++;
    }
    for (int i = 0; i < mediumToAssign && assignedCount < needed && i < mediumCount; i++) {
        y->moves[assignedCount].move = mediumMoves[i]; // 중급기 장착
        y->moves[assignedCount].currentPP = mediumMoves[i].pp;
        assignedCount++;
    }
    for (int i = 0; i < advancedToAssign && assignedCount < needed && i < advancedCount; i++) {
        y->moves[assignedCount].move = advancedMoves[i]; // 고급기 장착
        y->moves[assignedCount].currentPP = advancedMoves[i].pp;
        assignedCount++;
    }

    y->moveCount = assignedCount; // 실제 장착된 기술 개수 저장

    // 남은 슬롯이 있으면 추가로 채움(중복 방지)
    if (y->moveCount < needed) {
        Move* allMoves[] = {basicMoves, mediumMoves, advancedMoves}; // 모든 등급 기술 배열
        int counts[] = {basicCount, mediumCount, advancedCount}; // 각 등급별 개수
        int currentIndices[] = {basicToAssign, mediumToAssign, advancedToAssign}; // 각 등급별 현재 인덱스

        for (int i = 0; y->moveCount < needed; i++) {
            bool added = false; // 추가 여부 플래그
            for (int j = 0; j < 3; j++) {
                if (currentIndices[j] < counts[j]) {
                    bool alreadyAssigned = false; // 이미 장착된 기술인지 확인
                    for (int k = 0; k < y->moveCount; k++) {
                        if (strcmp(y->moves[k].move.name, allMoves[j][currentIndices[j]].name) == 0) {
                            alreadyAssigned = true;
                            break;
                        }
                    }
                    if (!alreadyAssigned) {
                        y->moves[y->moveCount].move = allMoves[j][currentIndices[j]]; // 기술 장착
                        y->moves[y->moveCount].currentPP = allMoves[j][currentIndices[j]].pp;
                        y->moveCount++;
                        added = true;
                    }
                    currentIndices[j]++;
                }
                if (y->moveCount >= needed) break;
            }
            if (!added) break; // 더 이상 추가할 기술이 없으면 종료
        }
    }

    // 배울 수 있지만 현재 장착하지 않은 기술을 forgottenMoves에 저장
    y->forgottenMoveCount = 0;
    for (int i = 0; i < y->learnableMoveCount; i++) {
        bool isAssigned = false;
        for (int j = 0; j < y->moveCount; j++) {
            if (strcmp(y->learnableMoves[i].name, y->moves[j].move.name) == 0) {
                isAssigned = true;
                break;
            }
        }

        if (!isAssigned) {
            if (level > 20) {  // 20레벨 이상에서 고급기술 배우기 가능
                 if (y->learnableMoves[i].grade == MOVE_BASIC || y->learnableMoves[i].grade == MOVE_MEDIUM) {
                    if (y->forgottenMoveCount < MAX_LEARNABLE_MOVES) {
                        y->forgottenMoves[y->forgottenMoveCount++] = y->learnableMoves[i]; // 미장착 기술 추가
                    }
                }
            } else if (level > 10) {
                if (y->learnableMoves[i].grade == MOVE_BASIC) {
                     if (y->forgottenMoveCount < MAX_LEARNABLE_MOVES) {
                        y->forgottenMoves[y->forgottenMoveCount++] = y->learnableMoves[i];
                    }
                }
            }
        }
    }

    y->learnedMoveCount = y->moveCount; // 실제 배운 기술 개수 저장
    for(int i=0; i<y->learnedMoveCount; ++i)
    {
        y->learnedMoves[i] = y->moves[i].move; // learnedMoves 배열에 복사
    }
}

Yokai* findYokaiByName(const char* name) {
    LOG_FUNCTION_EXECUTION("findYokaiByName"); // 함수 실행 로그 기록
    // 일반 요괴 리스트에서 검색
    for (int i = 0; i < yokaiListCount; i++) {
        if (strcmp(yokaiList[i].name, name) == 0) return &yokaiList[i]; // 이름이 일치하면 반환
    }
    // 보스 요괴 리스트에서 검색
    for (int i = 0; i < bossYokaiListCount; i++) {
        if (strcmp(bossYokaiList[i].name, name) == 0) return &bossYokaiList[i];
    }
    // 패러독스 요괴 리스트에서 검색
    for (int i = 0; i < paradoxYokaiListCount; i++) {
        if (strcmp(paradoxYokaiList[i].name, name) == 0) return &paradoxYokaiList[i];
    }
    return NULL; // 찾지 못하면 NULL 반환
}

// 스테이지 번호에 따른 레벨 범위 계산 함수
void calculateLevelRange(int stage, int* minLevel, int* maxLevel) {
    LOG_FUNCTION_EXECUTION("calculateLevelRange"); // 함수 실행 로그 기록
    // 기본 레벨 = 스테이지 번호
    int baseLevel = stage;
    
    // 레벨 범위 계산 공식
    // minLevel = baseLevel - floor(log2(stage + 1))
    // maxLevel = baseLevel + floor(log2(stage + 1))
    *minLevel = baseLevel - (int)floor(log2(stage + 1)); // 최소 레벨 계산
    *maxLevel = baseLevel + (int)floor(log2(stage + 1)); // 최대 레벨 계산
    
    // 보스 스테이지 (10의 배수)
    if (stage % 10 == 0) {
        *minLevel += 2; // 보스 스테이지는 레벨 상향
        *maxLevel += 2;
    }
    
    // 최소 레벨은 1
    if (*minLevel < 1) *minLevel = 1;
}

// 랜덤 요괴 생성 함수 (레벨 지정)
Yokai createRandomYokaiWithLevel(int level) {
    LOG_FUNCTION_EXECUTION("createRandomYokaiWithLevel"); // 함수 실행 로그 기록
    int index = rand() % yokaiListCount; // 요괴 리스트에서 랜덤 인덱스 선택
    Yokai newYokai = yokaiList[index]; // 해당 인덱스의 요괴 복사
    newYokai.level = level; // 레벨 설정
    newYokai.currentHP = calculateHP(&newYokai); // 현재 HP 계산
    newYokai.id = generateYokaiId(); // 고유 ID 부여
    newYokai.status = YOKAI_NORMAL; // 상태 초기화
    newYokai.exp = 0; // 경험치 초기화
    newYokai.yokaiInventoryCount = 0; // 인벤토리 개수 초기화
    newYokai.magnifierCount = 0; // 돋보기 개수 초기화
    
    assignRandomMoves(&newYokai, level); // 레벨에 맞는 기술 랜덤 배정
    return newYokai; // 완성된 요괴 반환
}

// 랜덤 보스 요괴 생성 함수 (레벨 지정)
Yokai createRandomBossYokaiWithLevel(int level) {
    LOG_FUNCTION_EXECUTION("createRandomBossYokaiWithLevel"); // 함수 실행 로그 기록
    int index = rand() % bossYokaiListCount; // 보스 요괴 리스트에서 랜덤 인덱스 선택
    Yokai newYokai = bossYokaiList[index]; // 해당 인덱스의 보스 요괴 복사
    newYokai.level = level; // 레벨 설정
    newYokai.currentHP = calculateHP(&newYokai); // 현재 HP 계산
    newYokai.id = generateYokaiId(); // 고유 ID 부여
    newYokai.status = YOKAI_NORMAL; // 상태 초기화
    assignRandomMoves(&newYokai, level); // 기술 랜덤 배정
    return newYokai; // 완성된 보스 요괴 반환
}

// 기본 레벨(1)로 랜덤 요괴 생성하는 함수
Yokai createRandomYokai() {
    LOG_FUNCTION_EXECUTION("createRandomYokai"); // 함수 실행 로그 기록
    return createRandomYokaiWithLevel(1); // 레벨 1로 생성
}

// 기본 레벨(1)로 랜덤 보스 요괴 생성하는 함수
Yokai createRandomBossYokai() {
    LOG_FUNCTION_EXECUTION("createRandomBossYokai"); // 함수 실행 로그 기록
    return createRandomBossYokaiWithLevel(1); // 레벨 1로 생성
}

Yokai createRandomParadoxYokaiWithLevel(int level) {
    LOG_FUNCTION_EXECUTION("createRandomParadoxYokaiWithLevel"); // 함수 실행 로그 기록
    int index = rand() % paradoxYokaiListCount; // 패러독스 요괴 리스트에서 랜덤 인덱스 선택
    Yokai newYokai = paradoxYokaiList[index]; // 해당 인덱스의 패러독스 요괴 복사
    newYokai.level = level; // 레벨 설정
    newYokai.currentHP = calculateHP(&newYokai); // 현재 HP 계산
    newYokai.id = generateYokaiId(); // 고유 ID 부여
    newYokai.status = YOKAI_NORMAL; // 상태 초기화
    newYokai.yokaiInventoryCount = 0; // 인벤토리 개수 초기화
    newYokai.magnifierCount = 0; // 돋보기 개수 초기화
    assignRandomMoves(&newYokai, level); // 기술 랜덤 배정
    return newYokai; // 완성된 패러독스 요괴 반환
}

// 요괴 정보 출력 함수
void printYokaiInfo(const Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("printYokaiInfo"); // 함수 실행 로그 기록
    printf("\n=== %s Lv.%d ===\n", yokai->name, yokai->level); // 이름, 레벨 출력
    printf("체력: %d\n", yokai->stamina); // 체력 출력
    printf("공격력: %d\n", yokai->attack); // 공격력 출력
    printf("방어력: %d\n", yokai->defense); // 방어력 출력
    printf("스피드: %d\n", yokai->speed); // 스피드 출력
    printf("상성: %s\n", typeNames[yokai->type]); // 상성 출력
    printf("설명: %s\n", typeDescriptions[yokai->type]); // 상성 설명 출력
    printf("\n배울 수 있는 기술:\n");
    for (int i = 0; i < yokai->learnableMoveCount; i++) {
        const char* colorCode;
        switch (yokai->learnableMoves[i].type) {
            case TYPE_EVIL_SPIRIT:
                colorCode = "\033[31m";  // 빨간색
                break;
            case TYPE_GHOST:
                colorCode = "\033[35m";  // 보라색
                break;
            case TYPE_MONSTER:
                colorCode = "\033[33m";  // 노란색
                break;
            case TYPE_HUMAN:
                colorCode = "\033[36m";  // 청록색
                break;
            case TYPE_ANIMAL:
                colorCode = "\033[32m";  // 초록색
                break;
            default:
                colorCode = "\033[0m";   // 기본색
        }
        printf("%d. %s%s\033[0m\n", i + 1, colorCode, yokai->learnableMoves[i].name); // 기술명 컬러 출력
    }
    printf("\n");
}

// 요괴 인벤토리 초기화
void initYokaiInventory(Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("initYokaiInventory"); // 함수 실행 로그 기록
    yokai->yokaiInventoryCount = 0; // 인벤토리 개수 초기화
    memset(yokai->yokaiInventory, 0, sizeof(yokai->yokaiInventory)); // 인벤토리 배열 초기화
}

// 요괴 인벤토리 출력
void printYokaiInventory(const Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("printYokaiInventory"); // 함수 실행 로그 기록
    printf("\n=== %s의 인벤토리 ===\n", yokai->name); // 요괴 이름 출력
    if (yokai->yokaiInventoryCount == 0) {
        printf("인벤토리가 비어있습니다.\n"); // 비어있으면 안내 출력
        return;
    }
    
    for (int i = 0; i < yokai->yokaiInventoryCount; i++) {
        const InventoryItem* invItem = &yokai->yokaiInventory[i]; // 현재 아이템 포인터
        int displayCount = invItem->count; // 표시할 개수
        if (strcmp(invItem->item.name, "찹살경단") == 0 && displayCount > 1) displayCount = 1; // 찹살경단은 1개만 표시
        printf("%d. %s (x%d) - %s\n", 
            i + 1,
            invItem->item.name,
            displayCount,
            invItem->item.desc); // 아이템 정보 출력
    }
    printf("\n");
}

// 요괴 인벤토리에 아이템 추가
bool addItemToYokaiInventory(Yokai* yokai, const Item* item) {
    LOG_FUNCTION_EXECUTION("addItemToYokaiInventory"); // 함수 실행 로그 기록
    // 기존 슬롯 탐색
    for (int i = 0; i < yokai->yokaiInventoryCount; i++) {
        if (strcmp(yokai->yokaiInventory[i].item.name, item->name) == 0) {
            int maxCount = 99; // 기본 최대 개수
            if (strcmp(item->name, "복숭아") == 0 || strcmp(item->name, "고대의 서적") == 0) maxCount = 5; // 특정 아이템은 5개 제한
            if (strcmp(item->name, "찹살경단") == 0) maxCount = 1; // 찹살경단은 1개 제한
            if (yokai->yokaiInventory[i].count < maxCount) {
                yokai->yokaiInventory[i].count++; // 개수 증가
                // 찹살경단은 count가 1을 초과할 경우 1로 강제 보정
                if (strcmp(item->name, "찹살경단") == 0 && yokai->yokaiInventory[i].count > 1) {
                    yokai->yokaiInventory[i].count = 1;
                }
                return true; // 추가 성공
            }
            return false; // 최대 개수 초과
        }
    }
    // 새 슬롯 추가
    if (yokai->yokaiInventoryCount >= INVENTORY_MAX) return false; // 인벤토리 가득 찬 경우
    yokai->yokaiInventory[yokai->yokaiInventoryCount].item = *item; // 새 슬롯에 아이템 추가
    yokai->yokaiInventory[yokai->yokaiInventoryCount].count = 1; // 개수 1로 설정
    yokai->yokaiInventoryCount++; // 인벤토리 개수 증가
    return true; // 추가 성공
}

// 요괴 인벤토리에서 아이템 제거
bool removeItemFromYokaiInventory(Yokai* yokai, int itemIndex) {
    LOG_FUNCTION_EXECUTION("removeItemFromYokaiInventory"); // 함수 실행 로그 기록
    if (itemIndex < 0 || itemIndex >= yokai->yokaiInventoryCount) {
        return false; // 인덱스 범위 초과
    }
    
    // 아이템 개수 감소
    yokai->yokaiInventory[itemIndex].count--;
    
    // 아이템이 모두 소진되었으면 인벤토리에서 제거
    if (yokai->yokaiInventory[itemIndex].count <= 0) {
        // 마지막 아이템을 현재 위치로 이동
        if (itemIndex < yokai->yokaiInventoryCount - 1) {
            yokai->yokaiInventory[itemIndex] = yokai->yokaiInventory[yokai->yokaiInventoryCount - 1];
        }
        yokai->yokaiInventoryCount--; // 인벤토리 개수 감소
    }
    return true; // 제거 성공
}

// 요괴 인벤토리 초기화
void clearYokaiInventory(Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("clearYokaiInventory"); // 함수 실행 로그 기록
    yokai->yokaiInventoryCount = 0; // 인벤토리 개수 초기화
    memset(yokai->yokaiInventory, 0, sizeof(yokai->yokaiInventory)); // 배열 초기화
}

// 레벨업 함수
void levelUpYokai(Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("levelUpYokai"); // 함수 실행 로그 기록
    yokai->level++; // 레벨 증가
    if (yokai->status != YOKAI_FAINTED) {  // 기절 상태가 아닐 때만 HP 증가
        yokai->currentHP = calculateHP(yokai); // HP 재계산
    }
} 