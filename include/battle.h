#ifndef BATTLE_H
#define BATTLE_H

#include "yokai.h"

typedef enum {
    BATTLE_FIGHT = 1,
    BATTLE_TALISMAN,
    BATTLE_CHECK_PARTY,
    BATTLE_RUN,
    BATTLE_CHECK_INVENTORY,
    BATTLE_SAVE_AND_EXIT
} BattleChoice;

int startBattle(const Yokai* enemy);
int showBattleMenu(const Yokai* enemy);
int handleBattleChoice(BattleChoice choice, Yokai* enemy);

#endif // BATTLE_H 