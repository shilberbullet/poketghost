#ifndef EXP_SYSTEM_H
#define EXP_SYSTEM_H

#include "yokai.h"

// 레벨업에 필요한 경험치 계산 함수
int calculateRequiredExp(int level);

// 전투 승리 시 획득 경험치 계산 함수 (기본)
int calculateBattleExp(const Yokai* enemy);

// 개별 요괴의 경험치 계산 함수 (새로 추가)
int calculateIndividualExp(const Yokai* enemy, const Yokai* player);

// 경험치 획득 및 레벨업 처리 함수
void gainExp(Yokai* yokai, int exp);

// 레벨업 처리 함수
void levelUp(Yokai* yokai);

#endif // EXP_SYSTEM_H 