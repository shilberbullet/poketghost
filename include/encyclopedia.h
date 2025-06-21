#ifndef ENCYCLOPEDIA_H
#define ENCYCLOPEDIA_H

#include "yokai.h"

// 도감 옵션 열거형
typedef enum {
    ENCYCLOPEDIA_ALL = 1,
    ENCYCLOPEDIA_BY_TYPE = 2,
    ENCYCLOPEDIA_CAUGHT = 3,
    ENCYCLOPEDIA_BACK = 4
} EncyclopediaOption;

// 도감 타입 필터 열거형
typedef enum {
    TYPE_FILTER_ALL = 0,
    TYPE_FILTER_MONSTER = 1,
    TYPE_FILTER_GHOST = 2,
    TYPE_FILTER_ANIMAL = 3,
    TYPE_FILTER_HUMAN = 4,
    TYPE_FILTER_EVIL_SPIRIT = 5
} TypeFilter;

// 요괴 인덱스로부터 실제 요괴를 가져오는 함수
Yokai* getYokaiByIndex(int index);

// 도감 표시 함수
void showEncyclopedia(void);

// 도감 메뉴 선택 처리 함수
int handleEncyclopediaChoice(EncyclopediaOption choice);

// 전체 요괴 목록 표시 함수
void showAllYokai(void);

// 타입별 요괴 목록 표시 함수
void showYokaiByType(void);

// 특정 타입의 요괴 목록 표시 함수
void showYokaiBySpecificType(int typeFilter);

// 잡은 요괴 목록 표시 함수
void showCaughtYokai(void);

// 요괴 상세 정보 표시 함수
void showYokaiDetail(int yokaiIndex);

// 타입 이름을 한글로 변환하는 함수
const char* getTypeNameInKorean(YokaiType type);

// 요괴를 잡았는지 확인하는 함수
int isYokaiCaught(int yokaiIndex);

// 잡은 요괴 수를 반환하는 함수
int getCaughtYokaiCount(void);

// 요괴를 잡은 것으로 표시하는 함수
void markYokaiAsCaught(int yokaiIndex);

// 잡은 요괴 정보를 초기화하는 함수
void resetCaughtYokai(void);

// 도감 관련 함수들
void saveCaughtYokaiData(void);
void loadCaughtYokaiData(void);

#endif // ENCYCLOPEDIA_H 