#ifndef SIKHYE_SYSTEM_H
#define SIKHYE_SYSTEM_H

#include "yokai.h"
#include "move.h"

// 식혜 아이템 사용 처리
void useSikhyeItem(const char* itemName, Yokai* targetYokai);

// 식혜 아이템 효과 적용
void applySikhyeEffect(Yokai* targetYokai, int ppAmount);

#endif // SIKHYE_SYSTEM_H 