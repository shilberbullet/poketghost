/**
 * @file party.h
 * @brief 포켓요괴 게임의 파티(동료) 관리 헤더 파일
 * @details 동료 요괴의 추가, 초기화, 성불 등 파티 관련 함수를 선언합니다.
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef PARTY_H
#define PARTY_H

#include "yokai.h" // 요괴 관련 헤더 - 요괴 구조체와 함수 사용

#define MAX_PARTY_SIZE 5 // 파티의 최대 동료 요괴 수 - 전투 참여 가능한 최대 요괴 수

// 동료 요괴 배열과 개수는 state 모듈에서 관리 - 전역 상태로 파티 정보 저장

/**
 * @brief 동료 요괴 파티를 초기화하는 함수
 * @details 게임 시작 시 파티를 빈 상태로 초기화하여 새로운 게임 준비
 */
void initParty();

/**
 * @brief 동료 요괴를 파티에 추가하는 함수
 * @param yokai 추가할 요괴 - 포획하거나 획득한 요괴 정보
 * @return 추가 성공 여부 - 1: 성공, 0: 실패 (파티가 가득 찬 경우)
 */
int addYokaiToParty(const Yokai* yokai);

/**
 * @brief 동료 요괴 목록을 출력하는 함수
 * @details 현재 파티에 있는 모든 요괴의 정보를 화면에 표시
 */
void printParty();

/**
 * @brief 요괴를 성불 처리하는 함수
 * @param index 성불시킬 요괴의 인덱스 - 파티 내 요괴의 위치 (0부터 시작)
 * @details 요괴를 파티에서 제거하고 게임에서 완전히 해방시킴
 */
void releaseYokai(int index);

/**
 * @brief 파티가 가득 찼을 때의 처리 함수
 * @param newYokai 새로 추가할 요괴 - 포획하려는 요괴 정보
 * @return 처리 결과 - 1: 교체 성공, 0: 교체 거부
 * @details 파티가 가득 찬 상태에서 새 요괴를 포획할 때 기존 요괴와 교체 여부 결정
 */
int handleFullParty(const Yokai* newYokai);

/**
 * @brief 모든 요괴의 PP를 초기화하는 함수
 * @details 전투 후 또는 특정 이벤트 후 모든 파티 요괴의 기술 PP를 최대값으로 복원
 */
void resetAllYokaiPP();

#endif // PARTY_H 