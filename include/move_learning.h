#ifndef MOVE_LEARNING_H
#define MOVE_LEARNING_H

#include "yokai.h"
#include "move.h"
#include <stdbool.h>

// 기술 학습 시스템 초기화
void initMoveLearningSystem();

// 레벨업 시 새로운 기술을 배울 수 있는지 확인
bool canLearnNewMove(const Yokai* yokai);

// 새로운 기술 학습 시도
bool tryLearnNewMove(Yokai* yokai);

// 기술 잊기
bool forgetMove(Yokai* yokai, int moveIndex);

// 기술 학습 메뉴 표시
void showMoveLearningMenu(Yokai* yokai);

// 기술 목록 출력
void printAvailableMoves(const Yokai* yokai);

// 기술 학습 확률 계산 (레벨에 따라 10% 확률)
bool calculateLearningChance(const Yokai* yokai);

#endif // MOVE_LEARNING_H 