#ifndef STATISTICS_H
#define STATISTICS_H

// 게임 통계 구조체
typedef struct {
    char user_name[50];
    int yokai_caught;
    int yokai_defeated;
    int stages_completed;
    int games_cleared;
} GameStatistics;

// 함수 선언
void load_total_statistics();
void save_total_statistics();
void update_total_statistics_on_save();
void display_statistics_screen();
void init_player_statistics();
void set_session_initial_statistics();

#endif // STATISTICS_H 