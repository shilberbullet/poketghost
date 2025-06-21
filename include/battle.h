/**
 * @file battle.h
 * @brief 포켓요괴 게임의 전투 시스템 헤더 파일
 * @details 전투 관련 함수, 구조체, 열거형을 선언합니다.
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef BATTLE_H
#define BATTLE_H

#define MAX_PARTY 6 // 파티의 최대 요괴 수

#include "types.h" // 기본 타입 정의
#include "yokai.h" // 요괴 관련 헤더
#include "stage.h" // 스테이지 관련 헤더

// 전투 메뉴 선택지
typedef enum {
    BATTLE_FIGHT = 1, // 전투 선택
    BATTLE_TALISMAN = 2, // 부적 사용 선택
    BATTLE_CHECK_PARTY = 3, // 파티 확인 선택
    BATTLE_SWITCH = 4, // 요괴 교체 선택
    BATTLE_RUN = 5, // 도망 선택
    BATTLE_CHECK_INVENTORY = 6, // 인벤토리 확인 선택
    BATTLE_SAVE_AND_EXIT = 7 // 저장 후 종료 선택
} BattleChoice; // 전투 중 선택할 수 있는 옵션들

// 현재 전투 중인 상대 요괴
extern Yokai currentEnemy; // 현재 전투 상대 요괴

// 전투 시작 함수
/**
 * @brief 전투를 시작하는 함수
 * @param enemy 전투할 상대 요괴
 * @return 전투 결과 (1: 승리, 0: 패배)
 */
int startBattle(const Yokai* enemy);

// 전투 메뉴 표시 함수 (1: 전투 종료, 0: 반복)
/**
 * @brief 전투 메뉴를 표시하는 함수
 * @param enemy 전투 상대 요괴
 * @return 전투 종료 여부 (1: 종료, 0: 계속)
 */
int showBattleMenu(const Yokai* enemy);

// 전투 선택 처리 함수 (1: 전투 종료, 0: 반복)
/**
 * @brief 전투 중 선택한 옵션을 처리하는 함수
 * @param choice 선택한 옵션
 * @param enemy 전투 상대 요괴
 * @return 전투 종료 여부 (1: 종료, 0: 계속)
 */
int handleBattleChoice(BattleChoice choice, Yokai* enemy);

/**
 * @brief 도망을 시도하는 함수
 * @return 도망 성공 여부 (1: 성공, 0: 실패)
 */
int tryToEscape(void);

// 전역 변수 선언
extern int turnCount; // 현재 턴 수
extern int lastYokaiIdx; // 마지막으로 사용한 요괴 인덱스
extern int participatedIdx[MAX_PARTY]; // 전투에 참여한 요괴 인덱스 배열
extern int participatedCount; // 전투에 참여한 요괴 수

/**
 * @brief 기절한 요괴를 처리하는 함수
 * @param faintedIdx 기절한 요괴의 인덱스
 * @return 처리 결과
 */
int handleFaintedYokai(int faintedIdx);

/**
 * @brief 파티 요괴를 선택하는 함수
 * @return 선택된 요괴 인덱스
 */
int selectPartyYokai();

/**
 * @brief 요괴의 기술을 선택하는 함수
 * @param yokai 기술을 선택할 요괴
 * @return 선택된 기술 인덱스
 */
int selectMove(const Yokai* yokai);

/**
 * @brief 인벤토리에서 부적을 선택하는 함수
 * @return 선택된 부적 인덱스
 */
int selectTalismanFromInventory();

// 요괴 교체 함수
/**
 * @brief 전투 중 요괴를 교체하는 함수
 * @return 교체 성공 여부 (1: 성공, 0: 실패)
 */
int switchYokai();

// 데미지 메시지 출력 함수
/**
 * @brief 데미지 메시지를 출력하는 함수
 * @param attacker 공격한 요괴
 * @param defender 공격받은 요괴
 * @param damage 입은 데미지
 */
void printDamageMessage(Yokai* attacker, Yokai* defender, int damage);

/**
 * @brief 파티 전체에 복숭아 치료 효과를 적용하는 함수
 */
void applyPeachHealingToParty();

#endif // BATTLE_H 