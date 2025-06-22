// 표준 입출력 함수를 위한 헤더
#include <stdio.h> // 표준 입출력 함수
// 메모리 할당을 위한 헤더
#include <stdlib.h> // 표준 라이브러리 함수
#include "escape.h" // 도망치기 헤더
#include "text.h" // 텍스트 출력 헤더
#include "../core/state.h" // 게임 상태 헤더
#include "logger.h" // 로거 헤더

// 보스 스테이지에서 도망치기 가능 여부 확인 함수
int canEscapeFromStage(void) { // 도망치기 가능 여부 확인 함수 시작
    LOG_FUNCTION_EXECUTION("canEscapeFromStage"); // 함수 실행 로그 기록
    // 10의 배수 스테이지(보스 스테이지) 또는 파이널 스테이지(81스테이지 이상)에서는 도망칠 수 없음
    if (gStage.stageNumber % 10 == 0 || gStage.stageNumber >= 81) { // 보스 스테이지이거나 파이널 스테이지인 경우
        printTextAndWait("\n알 수 없는 힘이 도망치지 못하게 합니다!"); // 도망치기 불가 메시지 출력
        return 0; // 도망치기 불가 반환
    }
    return 1; // 도망치기 가능 반환
}

// 도망치기 시도 함수
int tryToEscape(void) { // 도망치기 시도 함수 시작
    LOG_FUNCTION_EXECUTION("tryToEscape"); // 함수 실행 로그 기록
    // 보스 스테이지에서는 도망칠 수 없음
    if (!canEscapeFromStage()) { // 도망치기 불가능한 경우
        return ESCAPE_FAIL; // 도망치기 실패 반환
    }

    // 50% 확률로 도망치기 성공
    if (rand() % 2 == 0) { // 50% 확률로 성공
        printTextAndWait("\n도망치는데 성공했습니다!"); // 성공 메시지 출력
        return ESCAPE_SUCCESS; // 도망치기 성공 반환
    } else { // 50% 확률로 실패
        printTextAndWait("\n도망치는데 실패했습니다!"); // 실패 메시지 출력
        return ESCAPE_FAIL; // 도망치기 실패 반환
    }
} 