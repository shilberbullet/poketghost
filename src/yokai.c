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
    int lineCount = 0;
    while (fgets(line, sizeof(line), file)) {
        lineCount++;
        if (strstr(line, "# 보스 요괴")) { 
            isBoss = 1; 
            isParadox = 0; 
            LOG_FUNCTION_EXECUTION("보스 요괴 섹션 시작");
            continue; 
        }
        if (strstr(line, "# 패러독스 요괴")) { 
            isParadox = 1; 
            isBoss = 0; 
            LOG_FUNCTION_EXECUTION("패러독스 요괴 섹션 시작");
            continue; 
        }
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
            char logBuffer[512];
            if (isParadox && paradoxYokaiListCount < MAX_PARADOX_YOKAI) {
                y = &paradoxYokaiList[paradoxYokaiListCount++];
                sprintf(logBuffer, "패러독스 요괴 로드: %s (인덱스: %d)", name, paradoxYokaiListCount);
            } else if (!isBoss && yokaiListCount < MAX_YOKAI) {
                y = &yokaiList[yokaiListCount++];
                sprintf(logBuffer, "일반 요괴 로드: %s (인덱스: %d)", name, yokaiListCount);
            } else if (isBoss && bossYokaiListCount < MAX_BOSS_YOKAI) {
                y = &bossYokaiList[bossYokaiListCount++];
                sprintf(logBuffer, "보스 요괴 로드: %s (인덱스: %d)", name, bossYokaiListCount);
            } else {
                sprintf(logBuffer, "요괴 로드 실패: %s (배열 가득참)", name);
                LOG_FUNCTION_EXECUTION(logBuffer);
                continue;
            }
            LOG_FUNCTION_EXECUTION(logBuffer);
            
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
    
    // 로드된 요괴 수 로그 출력
    char logBuffer[256];
    sprintf(logBuffer, "로드된 요괴 수: 일반=%d, 보스=%d, 패러독스=%d, 총=%d", 
            yokaiListCount, bossYokaiListCount, paradoxYokaiListCount, 
            yokaiListCount + bossYokaiListCount + paradoxYokaiListCount);
    LOG_FUNCTION_EXECUTION(logBuffer);
}

// learnableMoves에서 랜덤 4개를 moves에 복사하는 함수
void assignRandomMoves(Yokai* y, int level) {
    LOG_FUNCTION_EXECUTION("assignRandomMoves");

    // 일반 요괴들의 기존 로직
    Move basicMoves[MAX_LEARNABLE_MOVES];
    int basicCount = 0;
    Move mediumMoves[MAX_LEARNABLE_MOVES];
    int mediumCount = 0;
    Move advancedMoves[MAX_LEARNABLE_MOVES];
    int advancedCount = 0;

    for (int i = 0; i < y->learnableMoveCount; i++) {
        switch (y->learnableMoves[i].grade) {
            case MOVE_BASIC:
                basicMoves[basicCount++] = y->learnableMoves[i];
                break;
            case MOVE_MEDIUM:
                mediumMoves[mediumCount++] = y->learnableMoves[i];
                break;
            case MOVE_ADVANCED:
                advancedMoves[advancedCount++] = y->learnableMoves[i];
                break;
        }
    }

    // Fisher-Yates shuffle for each grade
    for (int i = basicCount - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Move temp = basicMoves[i]; basicMoves[i] = basicMoves[j]; basicMoves[j] = temp;
    }
    for (int i = mediumCount - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Move temp = mediumMoves[i]; mediumMoves[i] = mediumMoves[j]; mediumMoves[j] = temp;
    }
    for (int i = advancedCount - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Move temp = advancedMoves[i]; advancedMoves[i] = advancedMoves[j]; advancedMoves[j] = temp;
    }

    y->moveCount = 0;
    int needed = MAX_MOVES;

    int basicToAssign = 0;
    int mediumToAssign = 0;
    int advancedToAssign = 0;

    if (level < 10) {
        basicToAssign = 4;
    } else if (level < 30) {
        basicToAssign = 2;
        mediumToAssign = 2;
    } else {
        basicToAssign = 1;
        mediumToAssign = 2;
        advancedToAssign = 1;
    }

    // Assign moves based on calculated numbers
    int assignedCount = 0;
    for (int i = 0; i < basicToAssign && assignedCount < needed && i < basicCount; i++) {
        y->moves[assignedCount].move = basicMoves[i];
        y->moves[assignedCount].currentPP = basicMoves[i].pp;
        assignedCount++;
    }
    for (int i = 0; i < mediumToAssign && assignedCount < needed && i < mediumCount; i++) {
        y->moves[assignedCount].move = mediumMoves[i];
        y->moves[assignedCount].currentPP = mediumMoves[i].pp;
        assignedCount++;
    }
    for (int i = 0; i < advancedToAssign && assignedCount < needed && i < advancedCount; i++) {
        y->moves[assignedCount].move = advancedMoves[i];
        y->moves[assignedCount].currentPP = advancedMoves[i].pp;
        assignedCount++;
    }

    y->moveCount = assignedCount;

    // Fill remaining slots if any
    if (y->moveCount < needed) {
        Move* allMoves[] = {basicMoves, mediumMoves, advancedMoves};
        int counts[] = {basicCount, mediumCount, advancedCount};
        int currentIndices[] = {basicToAssign, mediumToAssign, advancedToAssign};

        for (int i = 0; y->moveCount < needed; i++) {
            bool added = false;
            for (int j = 0; j < 3; j++) {
                if (currentIndices[j] < counts[j]) {
                    bool alreadyAssigned = false;
                    for (int k = 0; k < y->moveCount; k++) {
                        if (strcmp(y->moves[k].move.name, allMoves[j][currentIndices[j]].name) == 0) {
                            alreadyAssigned = true;
                            break;
                        }
                    }
                    if (!alreadyAssigned) {
                        y->moves[y->moveCount].move = allMoves[j][currentIndices[j]];
                        y->moves[y->moveCount].currentPP = allMoves[j][currentIndices[j]].pp;
                        y->moveCount++;
                        added = true;
                    }
                    currentIndices[j]++;
                }
                if (y->moveCount >= needed) break;
            }
            if (!added) break; // No more moves to add
        }
    }

    // Populate forgotten moves
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
            if (level > 11) {  // 11레벨 이상에서 고급기술 배우기 가능
                 if (y->learnableMoves[i].grade == MOVE_BASIC || y->learnableMoves[i].grade == MOVE_MEDIUM) {
                    if (y->forgottenMoveCount < MAX_LEARNABLE_MOVES) {
                        y->forgottenMoves[y->forgottenMoveCount++] = y->learnableMoves[i];
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

    y->learnedMoveCount = y->moveCount;
    for(int i=0; i<y->learnedMoveCount; ++i)
    {
        y->learnedMoves[i] = y->moves[i].move;
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
    int index = rand() % yokaiListCount;
    Yokai newYokai = yokaiList[index];
    newYokai.level = level;
    newYokai.currentHP = calculateHP(&newYokai);
    newYokai.id = generateYokaiId();
    newYokai.status = YOKAI_NORMAL;
    newYokai.exp = 0;
    newYokai.yokaiInventoryCount = 0;
    newYokai.magnifierCount = 0;
    
    assignRandomMoves(&newYokai, level);
    return newYokai;
}

// 랜덤 보스 요괴 생성 함수 (레벨 지정)
Yokai createRandomBossYokaiWithLevel(int level) {
    LOG_FUNCTION_EXECUTION("createRandomBossYokaiWithLevel");
    int index = rand() % bossYokaiListCount;
    Yokai newYokai = bossYokaiList[index];
    newYokai.level = level;
    newYokai.currentHP = calculateHP(&newYokai);
    newYokai.id = generateYokaiId();
    newYokai.status = YOKAI_NORMAL;
    assignRandomMoves(&newYokai, level);
    return newYokai;
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
    int index = rand() % paradoxYokaiListCount;
    Yokai newYokai = paradoxYokaiList[index];
    newYokai.level = level;
    newYokai.currentHP = calculateHP(&newYokai);
    newYokai.id = generateYokaiId();
    newYokai.status = YOKAI_NORMAL;
    newYokai.yokaiInventoryCount = 0;
    newYokai.magnifierCount = 0;
    assignRandomMoves(&newYokai, level);
    return newYokai;
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
        int displayCount = invItem->count;
        if (strcmp(invItem->item.name, "찹살경단") == 0 && displayCount > 1) displayCount = 1;
        printf("%d. %s (x%d) - %s\n", 
            i + 1,
            invItem->item.name,
            displayCount,
            invItem->item.desc);
    }
    printf("\n");
}

// 요괴 인벤토리에 아이템 추가
bool addItemToYokaiInventory(Yokai* yokai, const Item* item) {
    LOG_FUNCTION_EXECUTION("addItemToYokaiInventory");
    // 기존 슬롯 탐색
    for (int i = 0; i < yokai->yokaiInventoryCount; i++) {
        if (strcmp(yokai->yokaiInventory[i].item.name, item->name) == 0) {
            int maxCount = 99;
            if (strcmp(item->name, "복숭아") == 0 || strcmp(item->name, "고대의 서적") == 0) maxCount = 5;
            if (strcmp(item->name, "찹살경단") == 0) maxCount = 1;
            if (yokai->yokaiInventory[i].count < maxCount) {
                yokai->yokaiInventory[i].count++;
                // 찹살경단은 count가 1을 초과할 경우 1로 강제 보정
                if (strcmp(item->name, "찹살경단") == 0 && yokai->yokaiInventory[i].count > 1) {
                    yokai->yokaiInventory[i].count = 1;
                }
                return true;
            }
            return false;
        }
    }
    // 새 슬롯 추가
    if (yokai->yokaiInventoryCount >= INVENTORY_MAX) return false;
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