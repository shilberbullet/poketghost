#ifndef YOKAI_H
#define YOKAI_H

#include <stdbool.h>
#include "types.h"
#include "move.h"

#define MAX_MOVES 4
#define MAX_LEARNABLE_MOVES 12
#define YOKAI_NAME_MAX 32
#define MAX_YOKAI 32
#define MAX_BOSS_YOKAI 8

// 요괴 상성 타입
typedef enum {
    TYPE_EVIL_SPIRIT,    // 악귀
    TYPE_GHOST,          // 원귀
    TYPE_MONSTER,        // 괴수형
    TYPE_HUMAN,          // 인간형
    TYPE_ANIMAL,         // 동물형
    TYPE_COUNT
} YokaiType;

// 상성 이름 배열
extern const char* typeNames[];

// 상성 설명 배열
extern const char* typeDescriptions[];

// 요괴 구조체
typedef struct {
    char name[YOKAI_NAME_MAX];
    int attack;         // 공격력
    int defense;        // 방어력
    int hp;            // 체력
    int speed;         // 스피드
    YokaiType type;    // 상성
    Move moves[MAX_MOVES];  // 실제 전투/동료가 가진 기술
    int moveCount;     // 현재 배운 기술 수
    Move learnableMoves[MAX_LEARNABLE_MOVES]; // 배울 수 있는 기술 목록
    int learnableMoveCount;
} Yokai;

// 요괴 이름 배열
extern const char* yokaiNames[];

// 요괴 이름 개수
extern const int YOKAI_COUNT;

extern Yokai yokaiList[MAX_YOKAI];
extern int yokaiListCount;

extern Yokai bossYokaiList[MAX_BOSS_YOKAI];
extern int bossYokaiListCount;

void loadYokaiFromFile(const char* filename);
Yokai* findYokaiByName(const char* name);

// 랜덤 요괴 생성 함수
Yokai createRandomYokai();

// 랜덤 보스 요괴 생성 함수
Yokai createRandomBossYokai();

// 요괴 정보 출력 함수
void printYokaiInfo(const Yokai* yokai);

// 상성 관계 확인 함수 (공격자의 공격력이 방어자에게 얼마나 효과적인지 반환)
float getTypeEffectiveness(YokaiType attacker, YokaiType defender);

#endif // YOKAI_H 