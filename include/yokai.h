#ifndef YOKAI_H
#define YOKAI_H

#include "types.h"
#include "move.h"

#define MAX_YOKAI 100
#define MAX_BOSS_YOKAI 50
#define YOKAI_NAME_MAX 32
#define YOKAI_DESC_MAX 512
#define MAX_LEARNABLE_MOVES 8
#define MAX_MOVES 4
#define TYPE_COUNT 5

typedef enum {
    TYPE_EVIL_SPIRIT,  // 악귀
    TYPE_GHOST,        // 원귀
    TYPE_MONSTER,      // 괴수형
    TYPE_HUMAN,        // 인간형
    TYPE_ANIMAL        // 동물형
} YokaiType;

typedef struct {
    char name[YOKAI_NAME_MAX];
    YokaiType type;
    int attack;
    int defense;
    int hp;
    int speed;
    char desc[YOKAI_DESC_MAX];  // 도감 설명 버퍼 크기 증가
    MoveInstance moves[MAX_MOVES];
    int moveCount;
    Move learnableMoves[MAX_LEARNABLE_MOVES];
    int learnableMoveCount;
    int level;  // 레벨 필드 추가
} Yokai;

// 함수 선언
YokaiType parseType(const char* typeStr);
void loadYokaiFromFile(const char* filename);
void assignRandomMoves(Yokai* y);
Yokai* findYokaiByName(const char* name);
Yokai createRandomYokai();
Yokai createRandomBossYokai();
void printYokaiInfo(const Yokai* yokai);
float getTypeEffectiveness(YokaiType attacker, YokaiType defender);
void adjustStatsByLevel(Yokai* yokai);
void calculateLevelRange(int stage, int* minLevel, int* maxLevel);
Yokai createRandomYokaiWithLevel(int level);
Yokai createRandomBossYokaiWithLevel(int level);

#endif // YOKAI_H 