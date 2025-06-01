#ifndef BATTLE_SYSTEM_H
#define BATTLE_SYSTEM_H

#include "yokai.h"

// 전투 시스템 초기화
void initBattleSystem();

// 전투 실행
int executeBattle(Yokai* attacker, Yokai* defender, int moveIndex);

// 전투 결과 처리
void handleBattleResult(Yokai* attacker, Yokai* defender, int result);

#endif // BATTLE_SYSTEM_H 