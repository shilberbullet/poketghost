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

typedef struct {
    char name[YOKAI_NAME_MAX];
    YokaiType type;     // 기술의 상성
    int power;          // 공격력
    int accuracy;       // 명중률 (0-100)
    int pp;             // 사용 가능 횟수 (PP)
    char description[256];  // 기술 설명
} Move;

typedef struct {
    Move move;
    int currentPP; // 현재 남은 PP
} MoveInstance;

const char* typeToString(YokaiType type);

#endif // TYPES_H 