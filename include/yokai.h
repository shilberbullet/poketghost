#ifndef YOKAI_H
#define YOKAI_H

#include "types.h"
#include <stdbool.h>
#include "move.h"
#include "item.h"

#define MAX_MOVES 4
#define MAX_LEARNABLE_MOVES 50
#define YOKAI_NAME_MAX 32
#define MAX_YOKAI 32
#define MAX_BOSS_YOKAI 8
#define YOKAI_DESC_MAX 128
#define MAX_PARADOX_YOKAI 16
#define MAX_MAGNIFIER_COUNT 5  // 최대 돋보기 개수

// 상성 이름 배열
extern const char* typeNames[];

// 상성 설명 배열
extern const char* typeDescriptions[];

// 요괴 구조체
typedef struct {
    unsigned long long id;  // 요괴의 고유 ID
    char name[YOKAI_NAME_MAX];
    int level;         // 레벨
    int exp;           // 경험치 필드
    int attack;         // 공격력
    int defense;        // 방어력
    int stamina;         // 체력 종족값
    int speed;         // 스피드
    float currentHP;    // 현재 HP
    YokaiType type;    // 상성
    YokaiStatus status; // 상태
    char desc[YOKAI_DESC_MAX];    // 도감설명
    MoveInstance moves[MAX_MOVES];  // 실제 전투/동료가 가진 기술
    int moveCount;     // 현재 배운 기술 수
    Move learnableMoves[MAX_LEARNABLE_MOVES]; // 배울 수 있는 기술 목록
    int learnableMoveCount;
    Move learnedMoves[MAX_LEARNABLE_MOVES];  // 배운 기술 목록
    int learnedMoveCount;                    // 배운 기술 개수
    int forgottenMoveCount;  // 잊은 기술 개수
    Move forgottenMoves[MAX_LEARNABLE_MOVES];  // 잊은 기술 목록
    // 요괴 인벤토리 추가
    InventoryItem yokaiInventory[INVENTORY_MAX]; // 요괴별 인벤토리
    int yokaiInventoryCount; // 요괴 인벤토리 아이템 개수
    int magnifierCount;  // 보유 중인 돋보기 개수
} Yokai;

// 요괴 이름 배열
extern const char* yokaiNames[];

// 요괴 이름 개수
extern const int YOKAI_COUNT;

extern Yokai yokaiList[MAX_YOKAI];
extern int yokaiListCount;

extern Yokai bossYokaiList[MAX_BOSS_YOKAI];
extern int bossYokaiListCount;

extern Yokai paradoxYokaiList[MAX_PARADOX_YOKAI];
extern int paradoxYokaiListCount;

void loadYokaiFromFile(const char* filename);
Yokai* findYokaiByName(const char* name);

// HP 계산 함수
float calculateHP(const Yokai* yokai);

// 랜덤 요괴 생성 함수
Yokai createRandomYokai();

// 랜덤 보스 요괴 생성 함수
Yokai createRandomBossYokai();

// 요괴 정보 출력 함수
void printYokaiInfo(const Yokai* yokai);

// 요괴 인벤토리 관련 함수들
void initYokaiInventory(Yokai* yokai);
void printYokaiInventory(const Yokai* yokai);
bool addItemToYokaiInventory(Yokai* yokai, const Item* item);
bool removeItemFromYokaiInventory(Yokai* yokai, int itemIndex);
void clearYokaiInventory(Yokai* yokai);

// 상성 관계 확인 함수 (공격자의 공격력이 방어자에게 얼마나 효과적인지 반환)
float getTypeEffectiveness(YokaiType attacker, YokaiType defender);

// 상성 효과를 계산하는 함수
float calculateTypeEffectiveness(YokaiType attackerType, YokaiType defenderType);

// 스테이지 번호에 따른 레벨 범위 계산 함수
void calculateLevelRange(int stage, int* minLevel, int* maxLevel);

// 랜덤 요괴 생성 함수 (레벨 지정)
Yokai createRandomYokaiWithLevel(int level);

// 랜덤 보스 요괴 생성 함수 (레벨 지정)
Yokai createRandomBossYokaiWithLevel(int level);

// 랜덤 패러독스 요괴 생성 함수 (레벨 지정)
Yokai createRandomParadoxYokaiWithLevel(int level);

#endif // YOKAI_H 