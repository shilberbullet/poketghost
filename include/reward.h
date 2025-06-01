#ifndef REWARD_H
#define REWARD_H

#include "types.h"
#include "stage.h"
#include "game.h"
#include "item.h"

// 전투 보상 계산
int calculateBattleReward(void);

// 아이템 보상 시스템
void itemRewardSystem(void);

// 새 게임 시작 시 static 변수 초기화 함수
void resetItemRewardSystem(void);

// 랜덤 아이템 생성 (내부용)
void getRandomItems(Item* items, int count);

#endif // REWARD_H 