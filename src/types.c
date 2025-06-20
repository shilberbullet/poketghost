// 타입 관련 함수와 상수 정의 헤더
#include "types.h"
#include "logger.h"

// 요괴 타입을 문자열로 변환하는 함수
const char* typeToString(YokaiType type) {
    LOG_FUNCTION_EXECUTION("typeToString");
    switch (type) {
        case TYPE_MONSTER: return "괴수형";
        case TYPE_ANIMAL: return "동물형";
        case TYPE_GHOST: return "원귀";
        case TYPE_HUMAN: return "인간형";
        case TYPE_EVIL_SPIRIT: return "악귀";
        default: return "알 수 없음";
    }
}

// 기술 등급을 문자열로 변환하는 함수
const char* gradeToString(MoveGrade grade) {
    LOG_FUNCTION_EXECUTION("gradeToString");
    switch (grade) {
        case MOVE_BASIC: return "초급";
        case MOVE_MEDIUM: return "중급";
        case MOVE_ADVANCED: return "고급";
        default: return "알 수 없음";
    }
} 