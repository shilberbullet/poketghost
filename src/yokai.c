#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yokai.h"
#include "move.h"

#ifndef YOKAI_DESC_MAX
#define YOKAI_DESC_MAX 256
#endif

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
    char line[32768];  // 한글 인코딩을 고려하여 버퍼 크기를 더 크게 증가
    int isBoss = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "# 보스 요괴")) { isBoss = 1; continue; }
        if (line[0] == '#' || line[0] == '\n') continue;
        
        // 줄바꿈 문자 제거
        line[strcspn(line, "\n")] = 0;
        
        // 마지막 쉼표 위치 찾기
        char* last_comma = strrchr(line, ',');
        if (!last_comma) continue;
        char* moves = last_comma + 1;
        *last_comma = '\0'; // desc와 moves 분리
        
        // 앞에서부터 차례로 파싱
        char* name = strtok(line, ",");
        char* type = strtok(NULL, ",");
        char* attack = strtok(NULL, ",");
        char* defense = strtok(NULL, ",");
        char* hp = strtok(NULL, ",");
        char* speed = strtok(NULL, ",");
        char* desc = strtok(NULL, ""); // 남은 전체를 desc로
        
        if (name && type && attack && defense && hp && speed && desc && moves) {
            Yokai* y;
            if (!isBoss && yokaiListCount < MAX_YOKAI) {
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
            y->hp = atoi(hp);
            y->speed = atoi(speed);
            
            // 도감 설명 복사 전에 버퍼 초기화
            memset(y->desc, 0, YOKAI_DESC_MAX);
            strncpy(y->desc, desc, YOKAI_DESC_MAX - 1);
            y->desc[YOKAI_DESC_MAX - 1] = '\0';
            
            // 기술 목록 파싱
            y->learnableMoveCount = 0;
            char* moveName = strtok(moves, ";");
            while (moveName && y->learnableMoveCount < MAX_LEARNABLE_MOVES) {
                Move* m = findMoveByName(moveName);
                if (m) {
                    y->learnableMoves[y->learnableMoveCount++] = *m;
                }
                moveName = strtok(NULL, ";");
            }
            y->moveCount = 0;
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
        y->moves[i].move = y->learnableMoves[idx[i]];
        y->moves[i].currentPP = y->learnableMoves[idx[i]].pp;
    }
}

Yokai* findYokaiByName(const char* name) {
    for (int i = 0; i < yokaiListCount; i++) {
        if (strcmp(yokaiList[i].name, name) == 0) return &yokaiList[i];
    }
    return NULL;
}

// 레벨에 따른 능력치 보정 함수
void adjustStatsByLevel(Yokai* yokai) {
    float levelMultiplier = 1.0f + (yokai->level - 1) * 0.1f; // 레벨당 10% 증가
    yokai->attack = (int)(yokai->attack * levelMultiplier);
    yokai->defense = (int)(yokai->defense * levelMultiplier);
    yokai->hp = (int)(yokai->hp * levelMultiplier);
    yokai->speed = (int)(yokai->speed * levelMultiplier);
}

// 스테이지 번호에 따른 레벨 범위 계산 함수
void calculateLevelRange(int stage, int* minLevel, int* maxLevel) {
    // 기본 레벨 범위: 스테이지 번호 ± 2
    *minLevel = stage - 2;
    *maxLevel = stage + 2;
    
    // 10의 배수 스테이지는 더 높은 레벨
    if (stage % 10 == 0) {
        *minLevel += 3;
        *maxLevel += 3;
    }
    
    // 최소 레벨은 1
    if (*minLevel < 1) *minLevel = 1;
}

// 랜덤 요괴 생성 함수 (레벨 지정)
Yokai createRandomYokaiWithLevel(int level) {
    int idx = rand() % yokaiListCount;
    Yokai y = yokaiList[idx];
    y.level = level;
    adjustStatsByLevel(&y);
    assignRandomMoves(&y);
    return y;
}

// 랜덤 보스 요괴 생성 함수 (레벨 지정)
Yokai createRandomBossYokaiWithLevel(int level) {
    int idx = rand() % bossYokaiListCount;
    Yokai y = bossYokaiList[idx];
    y.level = level;
    adjustStatsByLevel(&y);
    assignRandomMoves(&y);
    return y;
}

// 기존 함수들을 레벨을 고려하도록 수정
Yokai createRandomYokai() {
    return createRandomYokaiWithLevel(1);
}

Yokai createRandomBossYokai() {
    return createRandomBossYokaiWithLevel(1);
}

void printYokaiInfo(const Yokai* yokai) {
    printf("\n=== %s Lv.%d ===\n", yokai->name, yokai->level);
    printf("상성: %s\n", typeNames[yokai->type]);
    printf("설명: %s\n", typeDescriptions[yokai->type]);
    printf("공격력: %d\n", yokai->attack);
    printf("방어력: %d\n", yokai->defense);
    printf("체력: %d\n", yokai->hp);
    printf("스피드: %d\n", yokai->speed);
    printf("\n기술 목록:\n");
    for (int i = 0; i < yokai->moveCount; i++) {
        printf("%d. %s\n", i + 1, yokai->moves[i].move.name);
    }
    printf("\n");
}

float getTypeEffectiveness(YokaiType attacker, YokaiType defender) {
    return typeEffectivenessTable[attacker][defender];
} 