/**
 * @file statistics.h
 * @brief 포켓요괴 게임의 통계 시스템 헤더 파일
 * @details 게임 내 통계 구조체와 관련 함수 선언
 * @author 포켓요괴 개발팀
 * @version 5.3
 */

#ifndef STATISTICS_H
#define STATISTICS_H

/**
 * @brief 게임 통계 구조체
 * @details 플레이어의 이름, 요괴 포획/격파/스테이지/클리어 횟수 등 저장
 */
typedef struct {
    char user_name[50]; // 플레이어 이름
    int yokai_caught; // 포획한 요괴 수
    int yokai_defeated; // 쓰러트린 요괴 수
    int stages_completed; // 클리어한 스테이지 수
    int games_cleared; // 게임 클리어 횟수
} GameStatistics;

// 외부에서 접근 가능한 전역 통계 변수
extern GameStatistics total_stats; // 전체 통계 전역 변수

// 함수 선언
/** @brief 통계 데이터 파일에서 불러오기 */
void load_total_statistics();
/** @brief 통계 데이터 파일로 저장 */
void save_total_statistics();
/** @brief 저장 시 통계 갱신 */
void update_total_statistics_on_save();
/** @brief 통계 화면 출력 */
void display_statistics_screen();
/** @brief 플레이어 통계 초기화 */
void init_player_statistics();
/** @brief 세션 통계 초기화 */
void set_session_initial_statistics();

#endif // STATISTICS_H 