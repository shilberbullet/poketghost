#ifndef YOKAI_H
#define YOKAI_H

#include "move.h"

#define YOKAI_NAME_MAX 32
#define MAX_MOVES 4
#define MAX_LEARNABLE_MOVES 8
#define MAX_YOKAI 100
#define MAX_BOSS_YOKAI 10
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
    Move moves[MAX_MOVES];
    int moveCount;
    Move learnableMoves[MAX_LEARNABLE_MOVES];
    int learnableMoveCount;
    char desc[256];
    int level;
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

#endif // YOKAI_H 