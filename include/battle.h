#ifndef BATTLE_H
#define BATTLE_H

#define MAX_PARTY 6

#include "types.h"
#include "yokai.h"
#include "stage.h"

// 전투 메뉴 선택지
typedef enum {
    BATTLE_FIGHT = 1,
    BATTLE_TALISMAN = 2,
    BATTLE_CHECK_PARTY = 3,
    BATTLE_SWITCH = 4,
    BATTLE_RUN = 5,
    BATTLE_CHECK_INVENTORY = 6,
    BATTLE_SAVE_AND_EXIT = 7
} BattleChoice;

// 현재 전투 중인 상대 요괴
extern Yokai currentEnemy;

// 전투 시작 함수
int startBattle(const Yokai* enemy);

// 전투 메뉴 표시 함수 (1: 전투 종료, 0: 반복)
int showBattleMenu(const Yokai* enemy);

// 전투 선택 처리 함수 (1: 전투 종료, 0: 반복)
int handleBattleChoice(BattleChoice choice, Yokai* enemy);

int tryToEscape(void);

// 전역 변수 선언
extern int turnCount;
extern int lastYokaiIdx;
extern int participatedIdx[MAX_PARTY];
extern int participatedCount;

int handleFaintedYokai(int faintedIdx);

int selectPartyYokai();
int selectMove(const Yokai* yokai);
int selectTalismanFromInventory();

// 요괴 교체 함수
int switchYokai();

// 데미지 메시지 출력 함수
void printDamageMessage(Yokai* attacker, Yokai* defender, int damage);

#endif // BATTLE_H 