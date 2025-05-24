#ifndef TYPES_H
#define TYPES_H

#define YOKAI_NAME_MAX 32
#define TYPE_COUNT 5

// 요괴 타입 정의
typedef enum {
    TYPE_MONSTER,
    TYPE_ANIMAL,
    TYPE_GHOST,
    TYPE_HUMAN,
    TYPE_EVIL_SPIRIT
} YokaiType;

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
    int pp;             // 사용 가능 횟수 (PP)
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