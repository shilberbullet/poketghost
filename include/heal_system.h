/**
 * @file heal_system.h
 * @brief 포켓요괴 게임의 회복 시스템 헤더 파일
 * @details 요괴 회복, 기술 PP 회복 등 관련 함수 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef HEAL_SYSTEM_H
#define HEAL_SYSTEM_H

#include "yokai.h" // 요괴 관련 헤더
#include "item.h" // 아이템 관련 헤더

// 현재 사용 중인 아이템
extern const Item* currentItem; // 현재 회복에 사용되는 아이템

/**
 * @brief 회복 시스템을 초기화하는 함수
 */
void initHealSystem();

/**
 * @brief 회복 시스템을 정리하는 함수
 */
void cleanupHealSystem();

/**
 * @brief 요괴를 회복하는 함수
 * @param targetYokai 회복할 요괴
 * @return 회복 성공 여부
 */
int healYokai(Yokai* targetYokai);

/**
 * @brief 회복 가능한 요괴 목록을 표시하고 선택하는 함수
 * @return 선택된 요괴 포인터
 */
Yokai* selectYokaiToHeal();

/**
 * @brief 요괴의 모든 기술 PP를 회복하는 함수
 * @param targetYokai 회복할 요괴
 * @param ppAmount 회복할 PP 양
 */
void healMovePP(Yokai* targetYokai, int ppAmount);

/**
 * @brief 요괴의 단일 기술 PP를 회복하는 함수
 * @param targetYokai 회복할 요괴
 * @param moveIndex 회복할 기술 인덱스
 * @param ppAmount 회복할 PP 양
 */
void healSingleMovePP(Yokai* targetYokai, int moveIndex, int ppAmount);

/**
 * @brief 요괴의 모든 기술 PP를 회복하는 함수
 * @param targetYokai 회복할 요괴
 */
void healAllMovesPP(Yokai* targetYokai);

/**
 * @brief 회복할 기술을 선택하는 함수
 * @param targetYokai 기술을 선택할 요괴
 * @return 선택된 기술 인덱스
 */
int selectMoveToHeal(Yokai* targetYokai);

#endif // HEAL_SYSTEM_H 