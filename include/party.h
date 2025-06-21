/**
 * @file party.h
 * @brief 포켓요괴 게임의 파티(동료) 관리 헤더 파일
 * @details 동료 요괴의 추가, 초기화, 성불 등 파티 관련 함수를 선언합니다.
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef PARTY_H
#define PARTY_H

#include "yokai.h" // 요괴 관련 헤더

#define MAX_PARTY_SIZE 5 // 파티의 최대 동료 요괴 수

// 동료 요괴 배열과 개수는 state 모듈에서 관리

/**
 * @brief 동료 요괴 파티를 초기화하는 함수
 */
void initParty();

/**
 * @brief 동료 요괴를 파티에 추가하는 함수
 * @param yokai 추가할 요괴
 * @return 추가 성공 여부
 */
int addYokaiToParty(const Yokai* yokai);

/**
 * @brief 동료 요괴 목록을 출력하는 함수
 */
void printParty();

/**
 * @brief 요괴를 성불 처리하는 함수
 * @param index 성불시킬 요괴의 인덱스
 */
void releaseYokai(int index);

/**
 * @brief 파티가 가득 찼을 때의 처리 함수
 * @param newYokai 새로 추가할 요괴
 * @return 처리 결과
 */
int handleFullParty(const Yokai* newYokai);

/**
 * @brief 모든 요괴의 PP를 초기화하는 함수
 */
void resetAllYokaiPP();

#endif // PARTY_H 