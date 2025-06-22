// 표준 입출력 함수를 위한 헤더
#include <stdio.h>  // 표준 입출력 함수 헤더 포함
// 문자열 처리 함수를 위한 헤더
#include <string.h>  // 문자열 처리 함수 헤더 포함
// 식혜 시스템 관련 함수와 구조체 정의
#include "sikhye_system.h"  // 식혜 시스템 헤더 포함
// 텍스트 출력 관련 함수
#include "text.h"  // 텍스트 출력 헤더 포함
// 회복 시스템 관련 함수
#include "heal_system.h"  // 회복 시스템 헤더 포함
// 로깅 함수를 위한 헤더
#include "logger.h"  // 로깅 시스템 헤더 포함

// 식혜 아이템 사용 함수
// 아이템 이름과 대상 요괴를 받아 해당하는 효과를 적용
void useSikhyeItem(const char* itemName, Yokai* targetYokai) {  // 식혜 아이템 사용 함수 정의
    LOG_FUNCTION_EXECUTION("useSikhyeItem");  // 함수 실행 로그 기록
    // 대상 요괴가 선택되지 않은 경우
    if (targetYokai == NULL) {  // 대상 요괴가 NULL인 경우
        printText("\n회복할 요괴가 선택되지 않았습니다.");  // 오류 메시지 출력
        return;  // 함수 종료
    }

    // 미지근한 식혜: PP 5 회복
    if (strcmp(itemName, "미지근한 식혜") == 0) {  // 미지근한 식혜인 경우
        applySikhyeEffect(targetYokai, 5);  // PP 5 회복 효과 적용
    }
    // 시원한 식혜: 선택한 기술의 PP 최대 회복
    else if (strcmp(itemName, "시원한 식혜") == 0) {  // 시원한 식혜인 경우
        applySikhyeEffect(targetYokai, -1);  // 선택한 기술 PP 최대 회복 효과 적용
    }
    // 맛있는 식혜: 모든 기술의 PP 회복
    else if (strcmp(itemName, "맛있는 식혜") == 0) {  // 맛있는 식혜인 경우
        healAllMovesPP(targetYokai);  // 모든 기술 PP 회복 효과 적용
    }
}

// 식혜 효과 적용 함수
// 대상 요괴와 회복할 PP 양을 받아 효과를 적용
void applySikhyeEffect(Yokai* targetYokai, int ppAmount) {  // 식혜 효과 적용 함수 정의
    LOG_FUNCTION_EXECUTION("applySikhyeEffect");  // 함수 실행 로그 기록
    // PP 양이 -1인 경우 (시원한 식혜)
    if (ppAmount == -1) {  // PP 양이 -1인 경우 (시원한 식혜)
        // 선택한 기술의 PP를 최대치까지 회복
        healMovePP(targetYokai, -1);  // 선택한 기술 PP 최대 회복
    } else {  // PP 양이 지정된 값인 경우
        // 지정된 양만큼 PP 회복
        healMovePP(targetYokai, ppAmount);  // 지정된 양만큼 PP 회복
    }
} 