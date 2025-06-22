// 표준 입출력 함수를 위한 헤더
#include <stdio.h>  // 표준 입출력 함수 헤더 포함
// 메모리 할당, 문자열 처리, Sleep 함수 사용을 위한 헤더
#include <stdlib.h>  // 표준 라이브러리 함수 헤더 포함
#include <string.h>  // 문자열 처리 함수 헤더 포함
#include <windows.h>  // Sleep 함수를 위해 추가
#include "roguelite.h"  // 로그라이트 시스템 헤더 포함
#include "party.h"  // 파티 관리 헤더 포함
#include "game.h"  // 게임 시스템 헤더 포함
#include "text.h"  // 텍스트 출력 헤더 포함
#include "input.h"  // 입력 처리 헤더 포함
#include "yokai.h"  // 요괴 시스템 헤더 포함
#include "menu.h"  // 메인 메뉴를 위해 추가
#include "main.h"  // main 함수를 위해 추가
#include "savefile.h"  // 세이브 파일 삭제를 위해 추가
#include "../core/state.h"  // 게임 상태 헤더 포함
#include "logger.h"  // 로깅 시스템 헤더 포함
#include "dialogue.h"  // 대화 시스템 헤더 포함

// 모든 요괴가 기절했는지 확인하는 함수
bool areAllYokaiFainted() {  // 모든 요괴 기절 확인 함수 정의
    LOG_FUNCTION_EXECUTION("areAllYokaiFainted");  // 함수 실행 로그 기록
    // HP와 상태 동기화
    for (int i = 0; i < gPartyCount; i++) {  // 파티의 모든 요괴에 대해 반복
        if (gParty[i].currentHP > 0) {  // HP가 0보다 큰 경우
            gParty[i].status = YOKAI_NORMAL;  // 상태를 정상으로 설정
        } else {  // HP가 0인 경우
            gParty[i].status = YOKAI_FAINTED;  // 상태를 기절로 설정
        }
    }
    for (int i = 0; i < gPartyCount; i++) {  // 파티의 모든 요괴에 대해 반복
        if (gParty[i].status != YOKAI_FAINTED) {  // 기절하지 않은 요괴가 있는 경우
            return false;  // false 반환
        }
    }
    return true;  // 모든 요괴가 기절한 경우 true 반환
}

// 게임 초기화 함수
void resetGame() {  // 게임 초기화 함수 정의
    LOG_FUNCTION_EXECUTION("resetGame");  // 함수 실행 로그 기록
    // 파티 초기화
    initParty();  // 파티 초기화 함수 호출
    
    // 게임 상태 초기화
    resetGameState();  // 게임 상태 초기화 함수 호출
}

// 로그라이트 시스템 메인 함수
void handleRogueliteSystem() {  // 로그라이트 시스템 메인 함수 정의
    LOG_FUNCTION_EXECUTION("handleRogueliteSystem");  // 함수 실행 로그 기록
    printText("\n모든 요괴가 기절했습니다!\n");  // 기절 메시지 출력
    printText("더 이상 싸울 수 있는 요괴가 없습니다.\n");  // 패배 안내 메시지 출력
    startDialogue(30);  // 패배 대화 시작
    printTextAndWait("잠시 후 게임이 초기화됩니다...\n");  // 초기화 안내 메시지 출력
    fastSleep(500);  // 0.5초 대기
    // 세이브 파일 삭제
    removeSaveFile();  // 세이브 파일 삭제 함수 호출
    // 게임 상태 초기화
    resetGame();  // 게임 초기화 함수 호출
    startDialogue(35);  // 패배 대화 시작
    printText("\n게임이 초기화되었습니다.\n");  // 초기화 완료 메시지 출력
    printTextAndWait("새로운 모험을 시작하세요!\n");  // 새로운 모험 안내 메시지 출력
    fastSleep(500);  // 0.5초 대기
    // 초기화면으로 이동
    gGameState.isRunning = 0;  // 게임 루프 종료 플래그 설정
    main();  // 초기화면으로 이동
} 