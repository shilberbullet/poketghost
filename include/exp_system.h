/**
 * @file exp_system.h
 * @brief 포켓요괴 게임의 경험치 시스템 헤더 파일
 * @details 경험치 계산, 레벨업 처리 등의 함수 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef EXP_SYSTEM_H
#define EXP_SYSTEM_H

#include "yokai.h" // 요괴 관련 헤더

/**
 * @brief 레벨업에 필요한 경험치를 계산하는 함수
 * @param level 현재 레벨
 * @return 필요한 경험치
 */
int calculateRequiredExp(int level);

/**
 * @brief 전투 승리 시 획득 경험치를 계산하는 함수 (기본)
 * @param enemy 상대 요괴
 * @return 획득 경험치
 */
int calculateBattleExp(const Yokai* enemy);

/**
 * @brief 개별 요괴의 경험치를 계산하는 함수 (새로 추가)
 * @param enemy 상대 요괴
 * @param player 플레이어 요괴
 * @return 획득 경험치
 */
int calculateIndividualExp(const Yokai* enemy, const Yokai* player);

/**
 * @brief 경험치 획득 및 레벨업을 처리하는 함수
 * @param yokai 경험치를 받을 요괴
 * @param exp 획득할 경험치
 */
void gainExp(Yokai* yokai, int exp);

/**
 * @brief 레벨업을 처리하는 함수
 * @param yokai 레벨업할 요괴
 */
void levelUp(Yokai* yokai);

#endif // EXP_SYSTEM_H 