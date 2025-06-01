#ifndef BATTLE_SYSTEM_H
#define BATTLE_SYSTEM_H

#include "yokai.h"

// 전투 시스템 초기화
void initBattleSystem();

// 상성 효과 계산
float getTypeEffectiveness(YokaiType attackerType, YokaiType defenderType);

// 데미지 계산
float calculateDamage(const Yokai* attacker, const Yokai* defender, const Move* move);

// 전투 실행
int executeBattle(Yokai* attacker, Yokai* defender, int moveIndex);

// 턴제 전투 실행
int executeTurnBattle(Yokai* playerYokai, Yokai* enemyYokai, int playerMoveIndex);

// 전투 결과 처리
void handleBattleResult(Yokai* attacker, Yokai* defender, int result);

#endif // BATTLE_SYSTEM_H 