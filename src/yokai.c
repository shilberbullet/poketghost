#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yokai.h"
#include "move.h"

Yokai yokaiList[MAX_YOKAI];
int yokaiListCount = 0;

Yokai bossYokaiList[MAX_BOSS_YOKAI];
int bossYokaiListCount = 0;

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

YokaiType parseType(const char* typeStr) {
    for (int i = 0; i < TYPE_COUNT; i++) {
        if (strcmp(typeStr, typeNames[i]) == 0) return (YokaiType)i;
    }
    return TYPE_EVIL_SPIRIT;
}

void loadYokaiFromFile(const char* filename) {
    yokaiListCount = 0;
    bossYokaiListCount = 0;
    FILE* file = fopen(filename, "r");
    if (!file) return;
    char line[256];
    int isBoss = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "# 보스 요괴")) { isBoss = 1; continue; }
        if (line[0] == '#' || line[0] == '\n') continue;
        char* name = strtok(line, ",");
        char* type = strtok(NULL, ",");
        char* moves = strtok(NULL, "\n");
        if (name && type && moves) {
            Yokai* y;
            if (!isBoss && yokaiListCount < MAX_YOKAI) {
                y = &yokaiList[yokaiListCount++];
            } else if (isBoss && bossYokaiListCount < MAX_BOSS_YOKAI) {
                y = &bossYokaiList[bossYokaiListCount++];
            } else {
                continue;
            }
            strncpy(y->name, name, YOKAI_NAME_MAX);
            y->type = parseType(type);
            y->learnableMoveCount = 0;
            char* moveName = strtok(moves, ";");
            while (moveName && y->learnableMoveCount < MAX_LEARNABLE_MOVES) {
                Move* m = findMoveByName(moveName);
                if (m) {
                    y->learnableMoves[y->learnableMoveCount++] = *m;
                }
                moveName = strtok(NULL, ";");
            }
            y->attack = rand() % 50 + 50;
            y->defense = rand() % 40 + 40;
            y->hp = rand() % 100 + 100;
            y->moveCount = 0; // 실제 moves는 생성 시 랜덤 4개로 할당
        }
    }
    fclose(file);
}

// learnableMoves에서 랜덤 4개를 moves에 복사하는 함수
void assignRandomMoves(Yokai* y) {
    int idx[MAX_LEARNABLE_MOVES];
    for (int i = 0; i < y->learnableMoveCount; i++) idx[i] = i;
    // Fisher-Yates shuffle
    for (int i = y->learnableMoveCount - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = idx[i]; idx[i] = idx[j]; idx[j] = tmp;
    }
    y->moveCount = (y->learnableMoveCount < MAX_MOVES) ? y->learnableMoveCount : MAX_MOVES;
    for (int i = 0; i < y->moveCount; i++) {
        y->moves[i] = y->learnableMoves[idx[i]];
    }
}

Yokai* findYokaiByName(const char* name) {
    for (int i = 0; i < yokaiListCount; i++) {
        if (strcmp(yokaiList[i].name, name) == 0) return &yokaiList[i];
    }
    return NULL;
}

Yokai createRandomYokai() {
    int idx = rand() % yokaiListCount;
    Yokai y = yokaiList[idx];
    assignRandomMoves(&y);
    return y;
}

Yokai createRandomBossYokai() {
    int idx = rand() % bossYokaiListCount;
    Yokai y = bossYokaiList[idx];
    assignRandomMoves(&y);
    return y;
}

void printYokaiInfo(const Yokai* yokai) {
    printf("\n=== %s ===\n", yokai->name);
    printf("상성: %s\n", typeNames[yokai->type]);
    printf("설명: %s\n", typeDescriptions[yokai->type]);
    printf("공격력: %d\n", yokai->attack);
    printf("방어력: %d\n", yokai->defense);
    printf("체력: %d\n", yokai->hp);
    printf("\n기술 목록:\n");
    for (int i = 0; i < yokai->moveCount; i++) {
        printf("%d. %s\n", i + 1, yokai->moves[i].name);
    }
    printf("\n");
}

float getTypeEffectiveness(YokaiType attacker, YokaiType defender) {
    return typeEffectivenessTable[attacker][defender];
} 