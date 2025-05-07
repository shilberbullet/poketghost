<<<<<<< HEAD
#ifndef BOSS_MONSTERS_H
#define BOSS_MONSTERS_H

#include "game_db.h"

// 보스 요괴 구조체
typedef struct {
    const char* name;
    const char* type;
    int hp;
    int attack;
    int defense;
    const char* specialMove;
    const char* description;
} BossMonster;

// 보스 요괴 배열
extern BossMonster bossMonsters[];
extern const int bossMonsterCount;

// 보스 요괴 관련 함수
BossMonster* getRandomBossMonster();
int isBossStage(int stageNumber);

=======
#ifndef BOSS_MONSTERS_H
#define BOSS_MONSTERS_H

#include "game_db.h"

// 보스 요괴 구조체
typedef struct {
    const char* name;
    const char* type;
    int hp;
    int attack;
    int defense;
    const char* specialMove;
    const char* description;
} BossMonster;

// 보스 요괴 배열
extern BossMonster bossMonsters[];
extern const int bossMonsterCount;

// 보스 요괴 관련 함수
BossMonster* getRandomBossMonster();
int isBossStage(int stageNumber);

>>>>>>> a7ceffc377aad0d00ad4801ac661379991d93098
#endif // BOSS_MONSTERS_H 