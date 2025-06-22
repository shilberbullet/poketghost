#include "statistics.h"  // 통계 관련 헤더 파일
#include "game.h"  // 게임 관련 헤더 파일
#include "text.h"  // 텍스트 출력 관련 헤더 파일
#include "input.h"  // 입력 처리 관련 헤더 파일
#include "state.h"  // 게임 상태 관련 헤더 파일
#include "party.h"  // 파티 관련 헤더 파일
#include "encyclopedia.h"  // 도감 초기화를 위해 추가
#include <stdio.h>  // 표준 입출력 함수
#include <windows.h>  // Windows API 함수
#include <conio.h>  // 콘솔 입력 함수
#include <string.h>  // 문자열 처리 함수

#define STATISTICS_FILE "data/statistics.dat"  // 통계 파일 경로 정의

// 전역 통계 (파일에 저장되는 총계)
GameStatistics total_stats;  // 전체 통계 구조체 변수

// 세션 시작 시점의 플레이어 통계 (세이브 파일 로드 시점)
GameStatistics session_initial_stats;  // 세션 초기 통계 구조체 변수

// 전체 통계를 파일에서 로드하는 함수
void load_total_statistics() {  // 전체 통계 로드 함수 시작
    FILE* file = fopen(STATISTICS_FILE, "rb");  // 통계 파일을 바이너리 읽기 모드로 열기
    if (file) {  // 파일이 성공적으로 열린 경우
        fread(&total_stats, sizeof(GameStatistics), 1, file);  // 통계 데이터를 파일에서 읽기
        fclose(file);  // 파일 닫기
        gTotalCaughtCount = total_stats.yokai_caught;  // 전체 포획 수를 전역 변수에 설정
    } else {  // 파일이 없는 경우
        // 파일이 없으면 이름 물어보고 0으로 초기화
        printf("당신의 이름은 무엇 입니까? ");  // 사용자 이름 입력 요청
        fgets(total_stats.user_name, sizeof(total_stats.user_name), stdin);  // 사용자 이름 입력 받기
        // fgets는 개행 문자도 포함하므로 제거
        char *p = strchr(total_stats.user_name, '\n');  // 개행 문자 위치 찾기
        if (p) *p = '\0';  // 개행 문자를 null 문자로 교체

        total_stats.yokai_caught = 0;  // 포획한 요괴 수를 0으로 초기화
        total_stats.yokai_defeated = 0;  // 쓰러뜨린 요괴 수를 0으로 초기화
        total_stats.stages_completed = 0;  // 클리어한 스테이지 수를 0으로 초기화
        total_stats.games_cleared = 0;  // 게임 클리어 횟수를 0으로 초기화
        
        save_total_statistics();  // 새로 생성된 통계를 바로 저장
        gTotalCaughtCount = 0;  // 전체 포획 수를 0으로 초기화
    }
}

// 전체 통계를 파일에 저장하는 함수
void save_total_statistics() {  // 전체 통계 저장 함수 시작
    FILE* file = fopen(STATISTICS_FILE, "wb");  // 통계 파일을 바이너리 쓰기 모드로 열기
    if (file) {  // 파일이 성공적으로 열린 경우
        total_stats.yokai_caught = gTotalCaughtCount;  // 전체 포획 수를 통계에 반영
        fwrite(&total_stats, sizeof(GameStatistics), 1, file);  // 통계 데이터를 파일에 쓰기
        fclose(file);  // 파일 닫기
    }
}

// 저장 시 전체 통계를 업데이트하는 함수
void update_total_statistics_on_save() {  // 저장 시 통계 업데이트 함수 시작
    // 현재 플레이어 통계와 세션 시작 시 통계의 차이를 계산
    int caught_delta = gPlayer.stats.yokai_caught - session_initial_stats.yokai_caught;  // 포획 수 차이 계산
    int defeated_delta = gPlayer.stats.yokai_defeated - session_initial_stats.yokai_defeated;  // 쓰러뜨린 수 차이 계산
    int stages_delta = gPlayer.stats.stages_completed - session_initial_stats.stages_completed;  // 스테이지 클리어 수 차이 계산
    int cleared_delta = gPlayer.stats.games_cleared - session_initial_stats.games_cleared;  // 게임 클리어 수 차이 계산

    // 총계에 차이만큼 더해줌
    if (caught_delta > 0) total_stats.yokai_caught += caught_delta;  // 포획 수가 증가한 경우 총계에 추가
    if (defeated_delta > 0) total_stats.yokai_defeated += defeated_delta;  // 쓰러뜨린 수가 증가한 경우 총계에 추가
    if (stages_delta > 0) total_stats.stages_completed += stages_delta;  // 스테이지 클리어 수가 증가한 경우 총계에 추가
    if (cleared_delta > 0) total_stats.games_cleared += cleared_delta;  // 게임 클리어 수가 증가한 경우 총계에 추가

    // 변경된 총계를 파일에 저장
    save_total_statistics();  // 전체 통계 저장

    // 다음 저장을 위해 현재 통계를 세션 시작 통계로 업데이트
    session_initial_stats = gPlayer.stats;  // 현재 플레이어 통계를 세션 초기 통계로 복사
}

// 통계 화면을 표시하는 함수
void display_statistics_screen() {  // 통계 화면 표시 함수 시작
    system("cls");  // 화면 지우기
    char buffer[256];  // 텍스트 버퍼 선언
    
    sprintf(buffer, "==================== %s님의 통계 ====================", total_stats.user_name);  // 통계 제목 생성
    printText(buffer);  // 통계 제목 출력
    printText("\n\n");  // 빈 줄 출력

    sprintf(buffer, "  동료로 만든 요괴 수 : %d\n", gTotalCaughtCount);  // 포획한 요괴 수 정보 생성
    printText(buffer);  // 포획한 요괴 수 출력

    sprintf(buffer, "  쓰러트린 요괴 수   : %d\n", total_stats.yokai_defeated);  // 쓰러뜨린 요괴 수 정보 생성
    printText(buffer);  // 쓰러뜨린 요괴 수 출력

    sprintf(buffer, "  클리어한 스테이지  : %d\n", total_stats.stages_completed);  // 클리어한 스테이지 수 정보 생성
    printText(buffer);  // 클리어한 스테이지 수 출력

    sprintf(buffer, "  게임 클리어 횟수   : %d\n", total_stats.games_cleared);  // 게임 클리어 횟수 정보 생성
    printText(buffer);  // 게임 클리어 횟수 출력

    printText("\n");  // 빈 줄 출력
    printText("============================================\n\n");  // 구분선 출력
    
    printText("아무 키나 누르면 메인 메뉴로 돌아갑니다...");  // 안내 메시지 출력
    _getch();  // 키 입력 대기
}

// 플레이어 통계를 초기화하는 함수
void init_player_statistics() {  // 플레이어 통계 초기화 함수 시작
    // 새 게임 시작 시 플레이어 통계 및 세션 통계 초기화
    gPlayer.stats.yokai_caught = 0;  // 포획한 요괴 수를 0으로 초기화
    gPlayer.stats.yokai_defeated = 0;  // 쓰러뜨린 요괴 수를 0으로 초기화
    gPlayer.stats.stages_completed = 0;  // 클리어한 스테이지 수를 0으로 초기화
    gPlayer.stats.games_cleared = 0;  // 게임 클리어 횟수를 0으로 초기화
    session_initial_stats = gPlayer.stats;  // 세션 초기 통계를 현재 통계로 설정
}

// 세션 초기 통계를 설정하는 함수
void set_session_initial_statistics() {  // 세션 초기 통계 설정 함수 시작
    // 게임을 이어할 때 세션 통계 초기화
    session_initial_stats = gPlayer.stats;  // 세션 초기 통계를 현재 플레이어 통계로 설정
} 