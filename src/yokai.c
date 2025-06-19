#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>  // SYSTEMTIME 구조체를 위해 추가
#include "yokai.h"
#include "move.h"
#include "logger.h"
#include "text.h"

#ifndef YOKAI_DESC_MAX
#define YOKAI_DESC_MAX 256
#endif

Yokai yokaiList[MAX_YOKAI];
int yokaiListCount = 0;

Yokai bossYokaiList[MAX_BOSS_YOKAI];
int bossYokaiListCount = 0;

#define MAX_PARADOX_YOKAI 16
Yokai paradoxYokaiList[MAX_PARADOX_YOKAI];
int paradoxYokaiListCount = 0;

// 요괴 이름 배열
const char* yokaiNames[] = {
    "구미호", "도깨비", "오니", "텐구", "카파",
    "누에", "바케네코", "코다마", "츠치노코", "누라리횬"
};

// 상성 이름 배열
const char* typeNames[] = {
    "악귀", "원귀", "괴수형", "인간형", "동물형"
};

// 상성 설명 배열
const char* typeDescriptions[] = {
    "악한 기운을 가진 요괴입니다.",
    "원한을 가진 영혼이 변한 요괴입니다.",
    "괴물과 같은 모습을 한 요괴입니다.",
    "인간과 비슷한 모습을 한 요괴입니다.",
    "동물의 모습을 한 요괴입니다."
};

const int YOKAI_COUNT = sizeof(yokaiNames) / sizeof(yokaiNames[0]);

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
};

// 상성 효과를 계산하는 함수
float calculateTypeEffectiveness(YokaiType attackerType, YokaiType defenderType) {
    LOG_FUNCTION_EXECUTION("calculateTypeEffectiveness");
    return typeEffectivenessTable[attackerType][defenderType];
}

// 전역 변수로 다음 ID 값 저장
static unsigned long long nextYokaiId = 1;

// ID 생성 함수
static unsigned long long generateYokaiId() {
    LOG_FUNCTION_EXECUTION("generateYokaiId");
    // 현재 시간을 밀리초 단위로 가져옴
    SYSTEMTIME st;
    GetSystemTime(&st);
    
    // 시간 기반 ID 생성 (년월일시분초밀리초)
    unsigned long long timeBasedId = 
        (unsigned long long)st.wYear * 10000000000000ULL +
        (unsigned long long)st.wMonth * 100000000000ULL +
        (unsigned long long)st.wDay * 1000000000ULL +
        (unsigned long long)st.wHour * 10000000ULL +
        (unsigned long long)st.wMinute * 100000ULL +
        (unsigned long long)st.wSecond * 1000ULL +
        (unsigned long long)st.wMilliseconds;
    
    // 전역 카운터와 조합하여 고유성 보장
    return timeBasedId + (nextYokaiId++);
}

YokaiType parseType(const char* typeStr) {
    LOG_FUNCTION_EXECUTION("parseType");
    if (strcmp(typeStr, "EVIL_SPIRIT") == 0) return TYPE_EVIL_SPIRIT;
    if (strcmp(typeStr, "GHOST") == 0) return TYPE_GHOST;
    if (strcmp(typeStr, "MONSTER") == 0) return TYPE_MONSTER;
    if (strcmp(typeStr, "HUMAN") == 0) return TYPE_HUMAN;
    if (strcmp(typeStr, "ANIMAL") == 0) return TYPE_ANIMAL;
    return TYPE_EVIL_SPIRIT;  // 기본값
}

void loadYokaiFromFile(const char* filename) {
    LOG_FUNCTION_EXECUTION("loadYokaiFromFile");
    yokaiListCount = 0;
    bossYokaiListCount = 0;
    paradoxYokaiListCount = 0;
    FILE* file = fopen(filename, "r");
    if (!file) return;
    char line[32768];
    int isBoss = 0;
    int isParadox = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "# 보스 요괴")) { isBoss = 1; isParadox = 0; continue; }
        if (strstr(line, "# 패러독스 요괴")) { isParadox = 1; isBoss = 0; continue; }
        if (line[0] == '#' || line[0] == '\n') continue;
        line[strcspn(line, "\n")] = 0;
        char* last_comma = strrchr(line, ',');
        if (!last_comma) continue;
        char* moves = last_comma + 1;
        *last_comma = '\0';
        char* name = strtok(line, ",");
        char* type = strtok(NULL, ",");
        char* attack = strtok(NULL, ",");
        char* defense = strtok(NULL, ",");
        char* stamina = strtok(NULL, ",");
        char* speed = strtok(NULL, ",");
        char* desc = strtok(NULL, "");
        if (name && type && attack && defense && stamina && speed && desc && moves) {
            Yokai* y;
            if (isParadox && paradoxYokaiListCount < MAX_PARADOX_YOKAI) {
                y = &paradoxYokaiList[paradoxYokaiListCount++];
            } else if (!isBoss && yokaiListCount < MAX_YOKAI) {
                y = &yokaiList[yokaiListCount++];
            } else if (isBoss && bossYokaiListCount < MAX_BOSS_YOKAI) {
                y = &bossYokaiList[bossYokaiListCount++];
            } else {
                continue;
            }
            strncpy(y->name, name, YOKAI_NAME_MAX - 1);
            y->name[YOKAI_NAME_MAX - 1] = '\0';
            y->type = parseType(type);
            y->attack = atoi(attack);
            y->defense = atoi(defense);
            y->stamina = atoi(stamina);
            y->speed = atoi(speed);
            memset(y->desc, 0, YOKAI_DESC_MAX);
            strncpy(y->desc, desc, YOKAI_DESC_MAX - 1);
            y->desc[YOKAI_DESC_MAX - 1] = '\0';
            y->learnableMoveCount = 0;
            y->yokaiInventoryCount = 0;  // 인벤토리 개수 초기화
            char* moveName = strtok(moves, ";");
            while (moveName && y->learnableMoveCount < MAX_LEARNABLE_MOVES) {
                Move* m = findMoveByName(moveName);
                if (m) {
                    y->learnableMoves[y->learnableMoveCount++] = *m;
                } else {
                    // 기술을 찾지 못한 경우 기본 기술 생성
                    Move defaultMove;
                    strncpy(defaultMove.name, moveName, YOKAI_NAME_MAX - 1);
                    defaultMove.name[YOKAI_NAME_MAX - 1] = '\0';
                    defaultMove.type = TYPE_EVIL_SPIRIT;  // 기본 타입
                    defaultMove.power = 50;               // 기본 위력
                    defaultMove.accuracy = 90;            // 기본 명중률
                    defaultMove.pp = 10;                  // 기본 PP
                    strncpy(defaultMove.description, "기본 기술", 255);
                    defaultMove.description[255] = '\0';
                    defaultMove.grade = MOVE_BASIC;       // 기본 등급
                    y->learnableMoves[y->learnableMoveCount++] = defaultMove;
                }
                moveName = strtok(NULL, ";");
            }
            // 최소 1개의 기술은 보장
            if (y->learnableMoveCount == 0) {
                Move defaultMove;
                strncpy(defaultMove.name, "기본공격", YOKAI_NAME_MAX - 1);
                defaultMove.name[YOKAI_NAME_MAX - 1] = '\0';
                defaultMove.type = TYPE_EVIL_SPIRIT;
                defaultMove.power = 50;
                defaultMove.accuracy = 90;
                defaultMove.pp = 10;
                strncpy(defaultMove.description, "기본 기술", 255);
                defaultMove.description[255] = '\0';
                defaultMove.grade = MOVE_BASIC;
                y->learnableMoves[y->learnableMoveCount++] = defaultMove;
            }
            y->moveCount = 0;
        }
    }
    fclose(file);
}

// learnableMoves에서 랜덤 4개를 moves에 복사하는 함수
void assignRandomMoves(Yokai* y) {
    LOG_FUNCTION_EXECUTION("assignRandomMoves");
    int idx[MAX_LEARNABLE_MOVES];
    for (int i = 0; i < y->learnableMoveCount; i++) idx[i] = i;
    // Fisher-Yates shuffle
    for (int i = y->learnableMoveCount - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = idx[i]; idx[i] = idx[j]; idx[j] = tmp;
    }
    y->moveCount = (y->learnableMoveCount < MAX_MOVES) ? y->learnableMoveCount : MAX_MOVES;
    y->learnedMoveCount = 0;  // 배운 기술 개수 초기화
    for (int i = 0; i < y->moveCount; i++) {
        y->moves[i].move = y->learnableMoves[idx[i]];
        y->moves[i].currentPP = y->learnableMoves[idx[i]].pp;
        // 배운 기술 목록에 추가
        y->learnedMoves[y->learnedMoveCount++] = y->learnableMoves[idx[i]];
    }
}

Yokai* findYokaiByName(const char* name) {
    LOG_FUNCTION_EXECUTION("findYokaiByName");
    // 일반 요괴 리스트에서 검색
    for (int i = 0; i < yokaiListCount; i++) {
        if (strcmp(yokaiList[i].name, name) == 0) return &yokaiList[i];
    }
    // 보스 요괴 리스트에서 검색
    for (int i = 0; i < bossYokaiListCount; i++) {
        if (strcmp(bossYokaiList[i].name, name) == 0) return &bossYokaiList[i];
    }
    // 패러독스 요괴 리스트에서 검색
    for (int i = 0; i < paradoxYokaiListCount; i++) {
        if (strcmp(paradoxYokaiList[i].name, name) == 0) return &paradoxYokaiList[i];
    }
    return NULL;
}

// 스테이지 번호에 따른 레벨 범위 계산 함수
void calculateLevelRange(int stage, int* minLevel, int* maxLevel) {
    LOG_FUNCTION_EXECUTION("calculateLevelRange");
    // 기본 레벨 = 스테이지 번호
    int baseLevel = stage;
    
    // 레벨 범위 계산 공식
    // minLevel = baseLevel - floor(log2(stage + 1))
    // maxLevel = baseLevel + floor(log2(stage + 1))
    *minLevel = baseLevel - (int)floor(log2(stage + 1));
    *maxLevel = baseLevel + (int)floor(log2(stage + 1));
    
    // 보스 스테이지 (10의 배수)
    if (stage % 10 == 0) {
        *minLevel += 2;
        *maxLevel += 2;
    }
    
    // 최소 레벨은 1
    if (*minLevel < 1) *minLevel = 1;
}

// 랜덤 요괴 생성 함수 (레벨 지정)
Yokai createRandomYokaiWithLevel(int level) {
    LOG_FUNCTION_EXECUTION("createRandomYokaiWithLevel");
    int idx;
    // 도깨비를 제외한 요괴만 선택
    do {
        idx = rand() % yokaiListCount;
    } while (strcmp(yokaiList[idx].name, "도깨비") == 0);
    
    Yokai y = yokaiList[idx];
    y.id = generateYokaiId();
    y.level = level;
    y.exp = 0;
    y.currentHP = calculateHP(&y);
    y.status = YOKAI_NORMAL;
    y.yokaiInventoryCount = 0;  // 인벤토리 개수 초기화
    y.magnifierCount = 0;  // 돋보기 개수 초기화
    assignRandomMoves(&y);
    return y;
}

// 랜덤 보스 요괴 생성 함수 (레벨 지정)
Yokai createRandomBossYokaiWithLevel(int level) {
    LOG_FUNCTION_EXECUTION("createRandomBossYokaiWithLevel");
    int idx = rand() % bossYokaiListCount;
    Yokai y = bossYokaiList[idx];
    y.id = generateYokaiId();  // 고유 ID 부여
    y.level = level;
    y.status = YOKAI_NORMAL;  // 기본 상태는 정상
    y.currentHP = calculateHP(&y);  // calculateHP() 함수 사용
    assignRandomMoves(&y);
    return y;
}

// 기본 레벨(1)로 랜덤 요괴 생성하는 함수
Yokai createRandomYokai() {
    LOG_FUNCTION_EXECUTION("createRandomYokai");
    return createRandomYokaiWithLevel(1);
}

// 기본 레벨(1)로 랜덤 보스 요괴 생성하는 함수
Yokai createRandomBossYokai() {
    LOG_FUNCTION_EXECUTION("createRandomBossYokai");
    return createRandomBossYokaiWithLevel(1);
}

Yokai createRandomParadoxYokaiWithLevel(int level) {
    LOG_FUNCTION_EXECUTION("createRandomParadoxYokaiWithLevel");
    if (paradoxYokaiListCount == 0) {
        loadYokaiFromFile("data/yokai.txt");
    }
    int idx = rand() % paradoxYokaiListCount;
    Yokai y = paradoxYokaiList[idx];
    y.id = 0;
    y.level = level;
    y.status = YOKAI_NORMAL;
    y.currentHP = calculateHP(&y);
    y.yokaiInventoryCount = 0;  // 인벤토리 개수 초기화
    y.magnifierCount = 0;  // 돋보기 개수 초기화
    assignRandomMoves(&y);
    return y;
}

void printYokaiInfo(const Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("printYokaiInfo");
    printf("\n=== %s Lv.%d ===\n", yokai->name, yokai->level);
    printf("체력: %d\n", yokai->stamina);
    printf("공격력: %d\n", yokai->attack);
    printf("방어력: %d\n", yokai->defense);
    printf("스피드: %d\n", yokai->speed);
    printf("상성: %s\n", typeNames[yokai->type]);
    printf("설명: %s\n", typeDescriptions[yokai->type]);
    printf("\n기술 목록:\n");
    for (int i = 0; i < yokai->moveCount; i++) {
        printf("%d. %s\n", i + 1, yokai->moves[i].move.name);
    }
    printf("\n");
}

// 요괴 인벤토리 초기화
void initYokaiInventory(Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("initYokaiInventory");
    yokai->yokaiInventoryCount = 0;
    memset(yokai->yokaiInventory, 0, sizeof(yokai->yokaiInventory));
}

// 요괴 인벤토리 출력
void printYokaiInventory(const Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("printYokaiInventory");
    printf("\n=== %s의 인벤토리 ===\n", yokai->name);
    if (yokai->yokaiInventoryCount == 0) {
        printf("인벤토리가 비어있습니다.\n");
        return;
    }
    
    for (int i = 0; i < yokai->yokaiInventoryCount; i++) {
        const InventoryItem* invItem = &yokai->yokaiInventory[i];
        printf("%d. %s (x%d) - %s\n", 
            i + 1,
            invItem->item.name,
            invItem->count,
            invItem->item.desc);
    }
    printf("\n");
}

// 요괴 인벤토리에 아이템 추가
bool addItemToYokaiInventory(Yokai* yokai, const Item* item) {
    LOG_FUNCTION_EXECUTION("addItemToYokaiInventory");
    // 중복 아이템 확인
    for (int i = 0; i < yokai->yokaiInventoryCount; i++) {
        if (strcmp(yokai->yokaiInventory[i].item.name, item->name) == 0) {
            // 복숭아는 5개까지만 보유 가능
            if (strcmp(item->name, "복숭아") == 0) {
                if (yokai->yokaiInventory[i].count >= 5) {
                    return false;
                }
            } else {
                // 그 외 아이템은 99개까지
                if (yokai->yokaiInventory[i].count >= 99) {
                    return false;
                }
            }
            yokai->yokaiInventory[i].count++;
            return true;
        }
    }
    
    // 인벤토리가 가득 찼는지 확인
    if (yokai->yokaiInventoryCount >= INVENTORY_MAX) {
        return false;
    }
    
    // 새 아이템 추가
    yokai->yokaiInventory[yokai->yokaiInventoryCount].item = *item;
    yokai->yokaiInventory[yokai->yokaiInventoryCount].count = 1;
    yokai->yokaiInventoryCount++;
    return true;
}

// 요괴 인벤토리에서 아이템 제거
bool removeItemFromYokaiInventory(Yokai* yokai, int itemIndex) {
    LOG_FUNCTION_EXECUTION("removeItemFromYokaiInventory");
    if (itemIndex < 0 || itemIndex >= yokai->yokaiInventoryCount) {
        return false;
    }
    
    // 아이템 개수 감소
    yokai->yokaiInventory[itemIndex].count--;
    
    // 아이템이 모두 소진되었으면 인벤토리에서 제거
    if (yokai->yokaiInventory[itemIndex].count <= 0) {
        // 마지막 아이템을 현재 위치로 이동
        if (itemIndex < yokai->yokaiInventoryCount - 1) {
            yokai->yokaiInventory[itemIndex] = yokai->yokaiInventory[yokai->yokaiInventoryCount - 1];
        }
        yokai->yokaiInventoryCount--;
    }
    return true;
}

// 요괴 인벤토리 초기화
void clearYokaiInventory(Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("clearYokaiInventory");
    yokai->yokaiInventoryCount = 0;
    memset(yokai->yokaiInventory, 0, sizeof(yokai->yokaiInventory));
}

// 레벨업 함수
void levelUpYokai(Yokai* yokai) {
    LOG_FUNCTION_EXECUTION("levelUpYokai");
    yokai->level++;
    if (yokai->status != YOKAI_FAINTED) {  // 기절 상태가 아닐 때만 HP 증가
        yokai->currentHP = calculateHP(yokai);
    }
} 