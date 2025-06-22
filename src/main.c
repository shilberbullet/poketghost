// 표준 입출력 함수를 위한 헤더
#include <stdio.h>      // 표준 입출력 함수를 위한 헤더 (printf, scanf 등)
// 메모리 할당, 랜덤 함수 등을 위한 헤더
#include <stdlib.h>     // 메모리 할당, 랜덤 함수 등을 위한 헤더 (malloc, rand 등)
// 시간 관련 함수를 위한 헤더
#include <time.h>       // 시간 관련 함수를 위한 헤더 (time 함수)
// 게임 관련 함수와 구조체 정의
#include "game.h"       // 게임 관련 함수와 구조체 정의 (게임 초기화, 실행 등)
// 메인 메뉴 관련 함수와 구조체 정의
#include "main_menu.h"  // 메인 메뉴 관련 함수와 구조체 정의 (메뉴 표시, 선택 등)
// 입력 처리 관련 함수와 구조체 정의
#include "input.h"      // 입력 처리 관련 함수와 구조체 정의 (키보드 입력 처리)
// 게임 설정 관련 함수와 구조체 정의
#include "settings.h"   // 게임 설정 관련 함수와 구조체 정의 (게임 옵션 관리)
// 이동 관련 함수와 구조체 정의
#include "move.h"       // 이동 관련 함수와 구조체 정의 (요괴 기술 시스템)
// 요괴 관련 함수와 구조체 정의
#include "yokai.h"      // 요괴 관련 함수와 구조체 정의 (요괴 데이터 관리)
// 아이템 관련 함수와 구조체 정의
#include "item.h"       // 아이템 관련 함수와 구조체 정의 (아이템 시스템)
// 회복 시스템 관련 함수와 구조체 정의
#include "heal_system.h" // 회복 시스템 관련 함수와 구조체 정의 (HP, PP 회복)
// 식혜 시스템 관련 함수와 구조체 정의
#include "sikhye_system.h" // 식혜 시스템 관련 함수와 구조체 정의 (특수 회복 아이템)
// 지역 시스템 관련 함수와 구조체 정의
#include "region.h"       // 지역 시스템 관련 함수와 구조체 정의 (게임 지역 관리)
// 로거 관련 함수와 구조체 정의
#include "logger.h"       // 로거 관련 함수와 구조체 정의 (디버깅 및 로그 기록)
#include "statistics.h"   // 통계 관련 함수와 구조체 정의 (게임 통계 관리)
// 대화 시스템 관련 함수와 구조체 정의
#include "dialogue.h"     // 대화 시스템 관련 함수와 구조체 정의 (NPC 대화 시스템)

/**
 * @brief 게임의 메인 함수
 * @details 게임의 시작점으로, 모든 시스템을 초기화하고 메인 메뉴를 실행합니다.
 * @return 프로그램 종료 코드 (0: 정상 종료)
 */
int main(void) { // 메인 함수 시작 (프로그램의 진입점)
    LOG_FUNCTION_EXECUTION("main"); // 함수 실행 로그 기록 (디버깅용)
    
    // 콘솔 출력 인코딩을 UTF-8로 설정 (출력 숨김)
    system("chcp 65001 > nul"); // 한글 출력을 위한 인코딩 설정 (Windows 콘솔에서 한글 표시 가능)
    
    // 랜덤 시드를 현재 시간으로 초기화하여 매번 다른 랜덤 값이 나오도록 함
    srand((unsigned int)time(NULL)); // 랜덤 시드 초기화 (전투, 아이템 드롭 등에서 사용)
    
    // 로거 초기화
    initLogger(); // 로깅 시스템 초기화 (디버깅 및 오류 추적을 위한 로그 파일 생성)
    LOG_FUNCTION_EXECUTION("initLogger"); // 로거 초기화 로그 기록
    
    // 통계 데이터 로드
    load_total_statistics(); // 전체 통계 데이터 로드 (게임 진행 상황, 전투 기록 등)

    // 아이템 시스템을 초기화하여 게임에서 사용할 아이템들을 준비
    LOG_FUNCTION_EXECUTION("initItemSystem"); // 아이템 시스템 초기화 로그 기록
    initItemSystem(); // 아이템 시스템 초기화 (아이템 목록, 효과 등 준비)
    
    // 회복 시스템을 초기화하여 게임에서 사용할 회복 기능들을 준비
    LOG_FUNCTION_EXECUTION("initHealSystem"); // 회복 시스템 초기화 로그 기록
    initHealSystem(); // 회복 시스템 초기화 (HP, PP 회복 기능 준비)
    
    // 지역 시스템을 초기화하여 게임에서 사용할 지역 기능들을 준비
    LOG_FUNCTION_EXECUTION("initRegionSystem"); // 지역 시스템 초기화 로그 기록
    initRegionSystem(); // 지역 시스템 초기화 (게임 지역, 스테이지 정보 준비)
    
    // 대화 시스템을 초기화하여 게임에서 사용할 대화 기능들을 준비
    LOG_FUNCTION_EXECUTION("initDialogueSystem"); // 대화 시스템 초기화 로그 기록
    initDialogueSystem(); // 대화 시스템 초기화 (NPC 대화, 스토리 텍스트 준비)
    
    // 게임에 필요한 데이터 파일들을 로드
    LOG_FUNCTION_EXECUTION("loadMovesFromFile"); // 기술 데이터 로드 로그 기록
    loadMovesFromFile("data/move.txt");     // 이동 관련 데이터 로드 (요괴 기술 정보)
    LOG_FUNCTION_EXECUTION("loadYokaiFromFile"); // 요괴 데이터 로드 로그 기록
    loadYokaiFromFile("data/yokai.txt");    // 요괴 관련 데이터 로드 (요괴 기본 정보)
    LOG_FUNCTION_EXECUTION("loadItemsFromFile"); // 아이템 데이터 로드 로그 기록
    loadItemsFromFile("data/items.txt");    // 아이템 관련 데이터 로드 (아이템 정보)
    LOG_FUNCTION_EXECUTION("loadDialoguesFromFile"); // 대화 데이터 로드 로그 기록
    loadDialoguesFromFile("data/dialogues.txt"); // 대화 관련 데이터 로드 (NPC 대화 내용)
    
    // 게임 설정을 초기화하여 기본 설정값을 로드
    LOG_FUNCTION_EXECUTION("initSettings"); // 설정 초기화 로그 기록
    initSettings(); // 게임 설정 초기화 (사용자 설정, 기본값 로드)
    
    // 무한 게임 루프 (게임이 종료될 때까지 반복)
    while (1) { // 게임이 종료될 때까지 반복 (무한 루프)
        LOG_FUNCTION_EXECUTION("initGame"); // 게임 초기화 로그 기록
        initGame(); // 게임 상태를 초기화 (새 게임 시작 시 상태 리셋)
        LOG_FUNCTION_EXECUTION("showMainMenu"); // 메인 메뉴 표시 로그 기록
        showMainMenu(); // 메인 메뉴를 표시 (사용자 선택 대기)
    }
    
    // 프로그램 종료 전 정리 작업 (현재는 도달하지 않는 코드)
    LOG_FUNCTION_EXECUTION("cleanupHealSystem"); // 회복 시스템 정리 로그 기록
    cleanupHealSystem(); // 회복 시스템 정리 (메모리 해제)
    LOG_FUNCTION_EXECUTION("cleanupItemSystem"); // 아이템 시스템 정리 로그 기록
    cleanupItemSystem(); // 아이템 시스템 정리 (메모리 해제)
    LOG_FUNCTION_EXECUTION("cleanupDialogueSystem"); // 대화 시스템 정리 로그 기록
    cleanupDialogueSystem(); // 대화 시스템 정리 (메모리 해제)
    
    // 로거 정리
    LOG_FUNCTION_EXECUTION("cleanupLogger"); // 로거 정리 로그 기록
    cleanupLogger(); // 로거 정리 (로그 파일 닫기)
    
    return 0; // 프로그램 정상 종료 (0 반환)
} 