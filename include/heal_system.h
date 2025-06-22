#ifndef HEAL_SYSTEM_H           // 중복 포함 방지를 위한 전처리기 지시문 시작
#define HEAL_SYSTEM_H           // 중복 포함 방지를 위한 매크로 정의

#include "yokai.h" // 요괴 관련 헤더 (Yokai 구조체 및 관련 함수 선언 포함)
#include "item.h"  // 아이템 관련 헤더 (Item 구조체 및 관련 함수 선언 포함)

// 현재 사용 중인 아이템을 외부에서 참조할 수 있도록 extern으로 선언
extern const Item* currentItem; // 현재 회복에 사용되는 아이템을 가리키는 전역 포인터

/**
 * @brief 회복 시스템을 초기화하는 함수
 */
void initHealSystem(); // 회복 시스템의 내부 상태 및 리소스를 초기화하는 함수 선언

/**
 * @brief 회복 시스템을 정리하는 함수
 */
void cleanupHealSystem(); // 회복 시스템에서 사용한 리소스를 해제하는 함수 선언

/**
 * @brief 요괴를 회복하는 함수
 * @param targetYokai 회복할 요괴
 * @return 회복 성공 여부 (1: 성공, -1: 실패)
 */
int healYokai(Yokai* targetYokai); // 지정한 요괴의 HP를 회복하는 함수 선언

/**
 * @brief 회복 가능한 요괴 목록을 표시하고 선택하는 함수
 * @return 선택된 요괴 포인터 (Yokai*)
 */
Yokai* selectYokaiToHeal(); // 회복할 요괴를 사용자에게 선택받아 반환하는 함수 선언

/**
 * @brief 요괴의 모든 기술 PP를 회복하는 함수
 * @param targetYokai 회복할 요괴
 * @param ppAmount 회복할 PP 양 (-1이면 최대치로 회복)
 */
void healMovePP(Yokai* targetYokai, int ppAmount); // 요괴의 모든 기술 또는 특정 기술의 PP를 회복하는 함수 선언

/**
 * @brief 요괴의 단일 기술 PP를 회복하는 함수
 * @param targetYokai 회복할 요괴
 * @param moveIndex 회복할 기술 인덱스
 * @param ppAmount 회복할 PP 양
 */
void healSingleMovePP(Yokai* targetYokai, int moveIndex, int ppAmount); // 특정 기술의 PP만 회복하는 함수 선언

/**
 * @brief 요괴의 모든 기술 PP를 회복하는 함수
 * @param targetYokai 회복할 요괴
 */
void healAllMovesPP(Yokai* targetYokai); // 요괴가 가진 모든 기술의 PP를 최대치로 회복하는 함수 선언

/**
 * @brief 회복할 기술을 선택하는 함수
 * @param targetYokai 기술을 선택할 요괴
 * @return 선택된 기술 인덱스 (0부터 시작)
 */
int selectMoveToHeal(Yokai* targetYokai); // 사용자가 회복할 기술을 선택하도록 하는 함수 선언

#endif // HEAL_SYSTEM_H           // 중복 포함 방지를 위한 전처리기 지시문 종료 