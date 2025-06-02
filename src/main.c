// 표준 입출력 함수를 위한 헤더
#include <stdio.h>      // 표준 입출력 함수를 위한 헤더
// 메모리 할당, 랜덤 함수 등을 위한 헤더
#include <stdlib.h>     // 메모리 할당, 랜덤 함수 등을 위한 헤더
// 시간 관련 함수를 위한 헤더
#include <time.h>       // 시간 관련 함수를 위한 헤더
// 게임 관련 함수와 구조체 정의
#include "game.h"       // 게임 관련 함수와 구조체 정의
// 메인 메뉴 관련 함수와 구조체 정의
#include "main_menu.h"  // 메인 메뉴 관련 함수와 구조체 정의
// 입력 처리 관련 함수와 구조체 정의
#include "input.h"      // 입력 처리 관련 함수와 구조체 정의
// 게임 설정 관련 함수와 구조체 정의
#include "settings.h"   // 게임 설정 관련 함수와 구조체 정의
// 이동 관련 함수와 구조체 정의
#include "move.h"       // 이동 관련 함수와 구조체 정의
// 요괴 관련 함수와 구조체 정의
#include "yokai.h"      // 요괴 관련 함수와 구조체 정의
// 아이템 관련 함수와 구조체 정의
#include "item.h"       // 아이템 관련 함수와 구조체 정의
// 회복 시스템 관련 함수와 구조체 정의
#include "heal_system.h" // 회복 시스템 관련 함수와 구조체 정의
// 식혜 시스템 관련 함수와 구조체 정의
#include "sikhye_system.h" // 식혜 시스템 관련 함수와 구조체 정의

// 메인 함수
int main(void) {
    // 콘솔 출력 인코딩을 UTF-8로 설정하여 한글 출력이 가능하도록 함
    system("chcp 65001");
    
    // 랜덤 시드를 현재 시간으로 초기화하여 매번 다른 랜덤 값이 나오도록 함
    srand((unsigned int)time(NULL));
    
    // 아이템 시스템을 초기화하여 게임에서 사용할 아이템들을 준비
    initItemSystem();
    
    // 회복 시스템을 초기화하여 게임에서 사용할 회복 기능들을 준비
    initHealSystem();
    
    // 게임에 필요한 데이터 파일들을 로드
    loadMovesFromFile("data/move.txt");     // 이동 관련 데이터 로드
    loadYokaiFromFile("data/yokai.txt");    // 요괴 관련 데이터 로드
    loadItemsFromFile("data/items.txt");    // 아이템 관련 데이터 로드
    
    // 게임 설정을 초기화하여 기본 설정값을 로드
    initSettings();
    
    // 무한 게임 루프
    while (1) {
        initGame(); // 게임 상태를 초기화
        showMainMenu(); // 메인 메뉴를 표시
    }
    
    // 프로그램 종료 전 정리 작업
    cleanupHealSystem(); // 회복 시스템 정리
    cleanupItemSystem(); // 아이템 시스템 정리
    
    return 0; // 프로그램 정상 종료
} 