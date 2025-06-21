/**
 * @file state.h
 * @brief 포켓요괴 게임의 상태 관리 헤더 파일
 * @details 게임 상태, 플레이어, 스테이지, 파티 등의 전역 상태 관리
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef STATE_H
#define STATE_H

#include "game.h" // 게임 관련 헤더
#include "party.h" // 파티 관련 헤더
#include "stage.h" // 스테이지 관련 헤더

// 상태 접근 함수
/**
 * @brief 게임 상태를 가져오는 함수
 * @return 게임 상태 포인터
 */
GameState* getGameState();

/**
 * @brief 게임 상태를 설정하는 함수
 * @param state 설정할 게임 상태
 */
void setGameState(GameState state);

/**
 * @brief 플레이어 정보를 가져오는 함수
 * @return 플레이어 포인터
 */
Player* getPlayer();

/**
 * @brief 플레이어 정보를 설정하는 함수
 * @param player 설정할 플레이어 정보
 */
void setPlayer(Player player);

/**
 * @brief 현재 스테이지 정보를 가져오는 함수
 * @return 현재 스테이지 포인터
 */
StageInfo* getCurrentStage();

/**
 * @brief 현재 스테이지 정보를 설정하는 함수
 * @param stage 설정할 스테이지 정보
 */
void setCurrentStage(StageInfo stage);

/**
 * @brief 파티 요괴 배열을 가져오는 함수
 * @return 파티 요괴 배열 포인터
 */
Yokai* getParty();

/**
 * @brief 파티 요괴 수 포인터를 가져오는 함수
 * @return 파티 요괴 수 포인터
 */
int* getPartyCountPtr();

/**
 * @brief 파티 요괴 수를 설정하는 함수
 * @param count 설정할 파티 요괴 수
 */
void setPartyCount(int count);

// 전역 상태 접근을 위한 편의 매크로
#define gGameState   (*getGameState()) // 게임 상태 전역 변수
#define gPlayer      (*getPlayer()) // 플레이어 전역 변수
#define gStage       (*getCurrentStage()) // 현재 스테이지 전역 변수
#define gParty       (getParty()) // 파티 요괴 배열 전역 변수
#define gPartyCount  (*getPartyCountPtr()) // 파티 요괴 수 전역 변수

#endif // STATE_H
