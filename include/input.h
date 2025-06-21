/**
 * @file input.h
 * @brief 포켓요괴 게임의 입력 처리 헤더 파일
 * @details 사용자 입력 처리 관련 함수 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef INPUT_H
#define INPUT_H

/**
 * @brief 입력 버퍼를 비우는 함수
 * @details 이전 입력을 무시하고 새로운 입력을 받기 위해 사용
 */
void clearInputBuffer();

/**
 * @brief 안전한 정수 입력을 받는 함수
 * @return 입력받은 정수, 오류 시 -1
 */
int getIntInput();

/**
 * @brief 엔터키를 기다리는 함수
 * @details 사용자가 엔터를 누를 때까지 대기
 */
void waitForEnter();

/**
 * @brief 범위 내 정수 입력을 받는 함수
 * @param min 최소값
 * @param max 최대값
 * @return 입력받은 정수, 범위 밖이면 -1
 */
int getNumberInput(int min, int max);

#endif // INPUT_H 