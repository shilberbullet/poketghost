/**
 * @file move_learning.h
 * @brief 포켓요괴 게임의 기술 학습 시스템 헤더 파일
 * @details 기술 학습, 잊기, 메뉴 표시 등의 함수 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef MOVE_LEARNING_H
#define MOVE_LEARNING_H

#include "yokai.h" // 요괴 관련 헤더
#include "move.h" // 기술 관련 헤더
#include <stdbool.h> // 불린 타입 사용을 위한 헤더

/**
 * @brief 기술 학습 시스템을 초기화하는 함수
 */
void initMoveLearningSystem();

/**
 * @brief 레벨업 시 새로운 기술을 배울 수 있는지 확인하는 함수
 * @param yokai 확인할 요괴
 * @return 새로운 기술 학습 가능 여부
 */
bool canLearnNewMove(const Yokai* yokai);

/**
 * @brief 새로운 기술 학습을 시도하는 함수
 * @param yokai 기술을 배울 요괴
 * @return 학습 성공 여부
 */
bool tryLearnNewMove(Yokai* yokai);

/**
 * @brief 기술을 잊게 하는 함수
 * @param yokai 기술을 잊을 요괴
 * @param moveIndex 잊을 기술 인덱스
 * @return 잊기 성공 여부
 */
bool forgetMove(Yokai* yokai, int moveIndex);

/**
 * @brief 기술 학습 메뉴를 표시하는 함수
 * @param yokai 메뉴를 표시할 요괴
 */
void showMoveLearningMenu(Yokai* yokai);

/**
 * @brief 배울 수 있는 기술 목록을 출력하는 함수
 * @param yokai 기술 목록을 출력할 요괴
 */
void printAvailableMoves(const Yokai* yokai);

/**
 * @brief 기술 학습 확률을 계산하는 함수 (레벨에 따라 10% 확률)
 * @param yokai 확률을 계산할 요괴
 * @return 학습 성공 여부
 */
bool calculateLearningChance(const Yokai* yokai);

#endif // MOVE_LEARNING_H 