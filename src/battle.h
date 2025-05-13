#ifndef BATTLE_H
#define BATTLE_H

#include "yokai.h"

// 전투 메뉴 선택지
typedef enum {
    BATTLE_FIGHT = 1,
    BATTLE_TALISMAN,
    BATTLE_CHECK_PARTY,
    BATTLE_RUN,
    BATTLE_CHECK_INVENTORY,
    BATTLE_SAVE_AND_EXIT
} BattleChoice;

// 전투 시작 함수
void startBattle();

// 전투 메뉴 표시 함수 (1: 전투 종료, 0: 반복)
int showBattleMenu(const Yokai* enemy);

// 전투 선택 처리 함수 (1: 전투 종료, 0: 반복)
int handleBattleChoice(BattleChoice choice, const Yokai* enemy);

#endif // BATTLE_H 