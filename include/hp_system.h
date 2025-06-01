#ifndef HP_SYSTEM_H
#define HP_SYSTEM_H

#include "yokai.h"

// HP 계산 함수
float calculateHP(const Yokai* yokai);

// HP 바 출력 함수
void printHPBar(const Yokai* yokai);

// HP 상태 확인 함수
const char* getHPStatus(const Yokai* yokai);

// HP 바 길이 상수
#define HP_BAR_LENGTH 20

#endif // HP_SYSTEM_H 