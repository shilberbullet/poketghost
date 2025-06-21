/**
 * @file hp_system.h
 * @brief 포켓요괴 게임의 HP 시스템 헤더 파일
 * @details HP 계산, HP 바 출력, 상태 확인 등의 함수 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef HP_SYSTEM_H
#define HP_SYSTEM_H

#include "yokai.h" // 요괴 관련 헤더

/**
 * @brief 요괴의 최대 HP를 계산하는 함수
 * @param yokai HP를 계산할 요괴
 * @return 계산된 최대 HP
 */
float calculateHP(const Yokai* yokai);

/**
 * @brief 요괴의 HP 바를 출력하는 함수
 * @param yokai HP 바를 출력할 요괴
 */
void printHPBar(const Yokai* yokai);

/**
 * @brief 요괴의 HP 상태를 확인하는 함수
 * @param yokai 상태를 확인할 요괴
 * @return HP 상태 문자열
 */
const char* getHPStatus(const Yokai* yokai);

// HP 바 길이 상수
#define HP_BAR_LENGTH 20 // HP 바의 최대 길이

#endif // HP_SYSTEM_H 