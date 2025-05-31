#include <stdio.h>
#include <string.h>
#include "sikhye_system.h"
#include "text.h"
#include "heal_system.h"

void useSikhyeItem(const char* itemName, Yokai* targetYokai) {
    if (targetYokai == NULL) {
        printTextAndWait("\n회복할 요괴가 선택되지 않았습니다.");
        return;
    }

    if (strcmp(itemName, "미지근한 식혜") == 0) {
        applySikhyeEffect(targetYokai, 5);  // PP 5 회복
    }
    else if (strcmp(itemName, "시원한 식혜") == 0) {
        applySikhyeEffect(targetYokai, -1);  // 선택한 기술 PP 최대 회복
    }
    else if (strcmp(itemName, "맛있는 식혜") == 0) {
        healAllMovesPP(targetYokai);  // 모든 기술 PP 회복
    }
}

void applySikhyeEffect(Yokai* targetYokai, int ppAmount) {
    if (ppAmount == -1) {
        // 선택한 기술 PP 최대 회복
        healMovePP(targetYokai, -1);
    } else {
        // 지정된 양만큼 PP 회복
        healMovePP(targetYokai, ppAmount);
    }
} 