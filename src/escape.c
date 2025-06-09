// 표준 입출력 함수를 위한 헤더
#include <stdio.h>
// 메모리 할당을 위한 헤더
#include <stdlib.h>
#include "escape.h"
#include "text.h"

// 보스 스테이지에서 도망치기 가능 여부 확인 함수
int canEscapeFromStage(void) {
    // 10의 배수 스테이지(보스 스테이지)에서는 도망칠 수 없음
    if (gStage.stageNumber % 10 == 0) {
        printTextAndWait("\n알 수 없는 힘이 도망치지 못하게 합니다!");
        return 0;
    }
    return 1;
}

// 도망치기 시도 함수
int tryToEscape(void) {
    // 보스 스테이지에서는 도망칠 수 없음
    if (!canEscapeFromStage()) {
        return ESCAPE_FAIL;
    }

    // 50% 확률로 도망치기 성공
    if (rand() % 2 == 0) {
        printTextAndWait("\n도망치는데 성공했습니다!");
        return ESCAPE_SUCCESS;
    } else {
        printTextAndWait("\n도망치는데 실패했습니다!");
        return ESCAPE_FAIL;
    }
} 