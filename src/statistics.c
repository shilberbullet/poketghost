#include "statistics.h"
#include "game.h"
#include "text.h"
#include "input.h"
#include "state.h"
#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define STATISTICS_FILE "data/statistics.dat"

// 전역 통계 (파일에 저장되는 총계)
GameStatistics total_stats;

// 세션 시작 시점의 플레이어 통계 (세이브 파일 로드 시점)
GameStatistics session_initial_stats;

void load_total_statistics() {
    FILE* file = fopen(STATISTICS_FILE, "rb");
    if (file) {
        fread(&total_stats, sizeof(GameStatistics), 1, file);
        fclose(file);
    } else {
        // 파일이 없으면 0으로 초기화
        total_stats.yokai_caught = 0;
        total_stats.yokai_defeated = 0;
        total_stats.stages_completed = 0;
        total_stats.games_cleared = 0;
    }
}

void save_total_statistics() {
    FILE* file = fopen(STATISTICS_FILE, "wb");
    if (file) {
        fwrite(&total_stats, sizeof(GameStatistics), 1, file);
        fclose(file);
    }
}

void update_total_statistics_on_save() {
    // 현재 플레이어 통계와 세션 시작 시 통계의 차이를 계산
    int caught_delta = gPlayer.stats.yokai_caught - session_initial_stats.yokai_caught;
    int defeated_delta = gPlayer.stats.yokai_defeated - session_initial_stats.yokai_defeated;
    int stages_delta = gPlayer.stats.stages_completed - session_initial_stats.stages_completed;
    int cleared_delta = gPlayer.stats.games_cleared - session_initial_stats.games_cleared;

    // 총계에 차이만큼 더해줌
    if (caught_delta > 0) total_stats.yokai_caught += caught_delta;
    if (defeated_delta > 0) total_stats.yokai_defeated += defeated_delta;
    if (stages_delta > 0) total_stats.stages_completed += stages_delta;
    if (cleared_delta > 0) total_stats.games_cleared += cleared_delta;

    // 변경된 총계를 파일에 저장
    save_total_statistics();

    // 다음 저장을 위해 현재 통계를 세션 시작 통계로 업데이트
    session_initial_stats = gPlayer.stats;
}

void display_statistics_screen() {
    system("cls");
    char buffer[256];

    printText("=== 포켓요괴 ===\n\n");
    
    printTextAndWait("==================== 통계 ====================\n\n");

    sprintf(buffer, "  잡은 요괴 수       : %d\n", total_stats.yokai_caught);
    printTextAndWait(buffer);

    sprintf(buffer, "  쓰러트린 요괴 수   : %d\n", total_stats.yokai_defeated);
    printTextAndWait(buffer);

    sprintf(buffer, "  클리어한 스테이지  : %d\n", total_stats.stages_completed);
    printTextAndWait(buffer);

    sprintf(buffer, "  게임 클리어 횟수   : %d\n", total_stats.games_cleared);
    printTextAndWait(buffer);

    printTextAndWait("\n");
    printTextAndWait("============================================\n\n");
    
    printText("아무 키나 누르면 메인 메뉴로 돌아갑니다...");
    _getch();
}

void init_player_statistics() {
    // 새 게임 시작 시 플레이어 통계 및 세션 통계 초기화
    gPlayer.stats.yokai_caught = 0;
    gPlayer.stats.yokai_defeated = 0;
    gPlayer.stats.stages_completed = 0;
    gPlayer.stats.games_cleared = 0;
    session_initial_stats = gPlayer.stats;
}

void set_session_initial_statistics() {
    // 게임을 이어할 때 세션 통계 초기화
    session_initial_stats = gPlayer.stats;
} 