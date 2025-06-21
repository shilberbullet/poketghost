/**
 * @file state.c
 * @brief 포켓요괴 게임의 상태 관리 구현 파일
 * @details 게임 상태, 플레이어, 스테이지, 파티 등의 전역 상태 관리 구현
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#include "state.h" // 상태 관리 헤더

// 전역 상태 변수들 (static으로 외부에서 직접 접근 불가)
static GameState _gameState = {0}; // 게임 상태 전역 변수
static Player _player = {0}; // 플레이어 정보 전역 변수
static StageInfo _currentStage = {0}; // 현재 스테이지 정보 전역 변수
static Yokai _party[MAX_PARTY_SIZE]; // 파티 요괴 배열 전역 변수
static int _partyCount = 0; // 파티 요괴 수 전역 변수
static int _totalCaughtCount = 0; // 누적 포획 수 전역 변수

/**
 * @brief 게임 상태를 가져오는 함수
 * @return 게임 상태 포인터
 */
GameState* getGameState() {
    return &_gameState; // 게임 상태 포인터 반환
}

/**
 * @brief 게임 상태를 설정하는 함수
 * @param state 설정할 게임 상태
 */
void setGameState(GameState state) {
    _gameState = state; // 게임 상태 설정
}

/**
 * @brief 플레이어 정보를 가져오는 함수
 * @return 플레이어 포인터
 */
Player* getPlayer() {
    return &_player; // 플레이어 포인터 반환
}

/**
 * @brief 플레이어 정보를 설정하는 함수
 * @param player 설정할 플레이어 정보
 */
void setPlayer(Player player) {
    _player = player; // 플레이어 정보 설정
}

/**
 * @brief 현재 스테이지 정보를 가져오는 함수
 * @return 현재 스테이지 포인터
 */
StageInfo* getCurrentStage() {
    return &_currentStage; // 현재 스테이지 포인터 반환
}

/**
 * @brief 현재 스테이지 정보를 설정하는 함수
 * @param stage 설정할 스테이지 정보
 */
void setCurrentStage(StageInfo stage) {
    _currentStage = stage; // 현재 스테이지 정보 설정
}

/**
 * @brief 파티 요괴 배열을 가져오는 함수
 * @return 파티 요괴 배열 포인터
 */
Yokai* getParty() {
    return _party; // 파티 요괴 배열 포인터 반환
}

/**
 * @brief 파티 요괴 수 포인터를 가져오는 함수
 * @return 파티 요괴 수 포인터
 */
int* getPartyCountPtr() {
    return &_partyCount; // 파티 요괴 수 포인터 반환
}

/**
 * @brief 파티 요괴 수를 설정하는 함수
 * @param count 설정할 파티 요괴 수
 */
void setPartyCount(int count) {
    _partyCount = count; // 파티 요괴 수 설정
}

/**
 * @brief 누적 포획 수 포인터를 가져오는 함수
 * @return 누적 포획 수 포인터
 */
int* getTotalCaughtCountPtr() {
    return &_totalCaughtCount; // 누적 포획 수 포인터 반환
}

/**
 * @brief 누적 포획 수를 설정하는 함수
 * @param count 설정할 누적 포획 수
 */
void setTotalCaughtCount(int count) {
    _totalCaughtCount = count; // 누적 포획 수 설정
}
