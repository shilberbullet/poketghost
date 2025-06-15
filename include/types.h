#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#define YOKAI_NAME_MAX 32
#define TYPE_COUNT 5

typedef enum {
    TYPE_EVIL_SPIRIT,  // 악귀
    TYPE_GHOST,        // 원귀
    TYPE_MONSTER,      // 괴수형
    TYPE_HUMAN,        // 인간형
    TYPE_ANIMAL        // 동물형
} YokaiType;

// 요괴 상태 정의
typedef enum {
    YOKAI_NORMAL,     // 정상
    YOKAI_FAINTED,    // 기절
    YOKAI_RELEASED    // 성불
} YokaiStatus;

// 기술 등급 정의
typedef enum {
    MOVE_BASIC,    // 초급
    MOVE_MEDIUM,   // 중급
    MOVE_ADVANCED  // 고급
} MoveGrade;

typedef struct {
    char name[YOKAI_NAME_MAX];
    YokaiType type;     // 기술의 상성
    int power;          // 공격력
    int accuracy;       // 명중률 (0-100)
    int pp;             // 최대 사용 가능 횟수 (PP)
    char description[256];  // 기술 설명
    MoveGrade grade;    // 기술 등급
} Move;

typedef struct {
    Move move;
    int currentPP; // 현재 남은 PP
} MoveInstance;

const char* typeToString(YokaiType type);
const char* gradeToString(MoveGrade grade);

#endif // TYPES_H 