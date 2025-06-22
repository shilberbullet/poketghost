// 타입 관련 함수와 상수 정의 헤더
#include "types.h"  // 타입 관련 헤더 포함
#include "logger.h"  // 로깅 시스템 헤더 포함

// 요괴 타입을 문자열로 변환하는 함수
const char* typeToString(YokaiType type) {  // 요괴 타입을 문자열로 변환하는 함수 정의
    LOG_FUNCTION_EXECUTION("typeToString");  // 함수 실행 로그 기록
    switch (type) {  // 요괴 타입에 따른 문자열 반환
        case TYPE_MONSTER: return "괴수형";  // 괴수형 타입 반환
        case TYPE_ANIMAL: return "동물형";  // 동물형 타입 반환
        case TYPE_GHOST: return "원귀";  // 원귀 타입 반환
        case TYPE_HUMAN: return "인간형";  // 인간형 타입 반환
        case TYPE_EVIL_SPIRIT: return "악귀";  // 악귀 타입 반환
        default: return "알 수 없음";  // 기본값 반환
    }
}

// 기술 등급을 문자열로 변환하는 함수
const char* gradeToString(MoveGrade grade) {  // 기술 등급을 문자열로 변환하는 함수 정의
    LOG_FUNCTION_EXECUTION("gradeToString");  // 함수 실행 로그 기록
    switch (grade) {  // 기술 등급에 따른 문자열 반환
        case MOVE_BASIC: return "초급";  // 초급 등급 반환
        case MOVE_MEDIUM: return "중급";  // 중급 등급 반환
        case MOVE_ADVANCED: return "고급";  // 고급 등급 반환
        default: return "알 수 없음";  // 기본값 반환
    }
} 