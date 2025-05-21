#include "types.h"

const char* typeToString(YokaiType type) {
    switch (type) {
        case TYPE_MONSTER: return "괴수형";
        case TYPE_ANIMAL: return "동물형";
        case TYPE_GHOST: return "원귀";
        case TYPE_HUMAN: return "인간형";
        case TYPE_EVIL_SPIRIT: return "악귀";
        default: return "알 수 없음";
    }
}

const char* gradeToString(MoveGrade grade) {
    switch (grade) {
        case MOVE_BASIC: return "초급";
        case MOVE_MEDIUM: return "중급";
        case MOVE_ADVANCED: return "고급";
        default: return "알 수 없음";
    }
} 