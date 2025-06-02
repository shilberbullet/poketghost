#ifndef HEAL_SYSTEM_H
#define HEAL_SYSTEM_H

#include "yokai.h"
#include "item.h"

// 현재 사용 중인 아이템
extern const Item* currentItem;

// 회복 시스템 초기화
void initHealSystem();

// 회복 시스템 정리
void cleanupHealSystem();

// 요괴 회복 처리
void healYokai(Yokai* targetYokai);

// 회복 가능한 요괴 목록 표시 및 선택
Yokai* selectYokaiToHeal();

// 기술 PP 회복 처리
void healMovePP(Yokai* targetYokai, int ppAmount);

// 단일 기술 PP 회복 처리
void healSingleMovePP(Yokai* targetYokai, int moveIndex, int ppAmount);

// 모든 기술 PP 회복 처리
void healAllMovesPP(Yokai* targetYokai);

// 기술 선택 함수
int selectMoveToHeal(Yokai* targetYokai);

#endif // HEAL_SYSTEM_H 