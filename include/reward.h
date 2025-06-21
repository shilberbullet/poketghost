/**
 * @file reward.h
 * @brief 포켓요괴 게임의 보상 시스템 헤더 파일
 * @details 전투 보상, 아이템 보상 시스템 함수 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef REWARD_H
#define REWARD_H

#include "types.h" // 기본 타입 정의
#include "stage.h" // 스테이지 관련 헤더
#include "game.h" // 게임 관련 헤더
#include "item.h" // 아이템 관련 헤더

/**
 * @brief 전투 보상을 계산하는 함수
 * @return 획득한 보상 금액
 */
int calculateBattleReward(void);

/**
 * @brief 아이템 보상 시스템을 실행하는 함수
 */
void itemRewardSystem(void);

/**
 * @brief 새 게임 시작 시 아이템 보상 시스템의 static 변수 초기화 함수
 */
void resetItemRewardSystem(void);

/**
 * @brief 랜덤 아이템을 생성하는 내부용 함수
 * @param items 생성된 아이템 배열
 * @param count 생성할 아이템 개수
 */
void getRandomItems(Item* items, int count);

#endif // REWARD_H 