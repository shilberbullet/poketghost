#ifndef STATE_H
#define STATE_H

#include "game.h"
#include "party.h"
#include "stage.h"

// 상태 접근 함수
GameState* getGameState();
void setGameState(GameState state);

Player* getPlayer();
void setPlayer(Player player);

StageInfo* getCurrentStage();
void setCurrentStage(StageInfo stage);

Yokai* getParty();
int* getPartyCountPtr();
void setPartyCount(int count);

// 전역 상태 접근을 위한 편의 매크로
#define gGameState   (*getGameState())
#define gPlayer      (*getPlayer())
#define gStage       (*getCurrentStage())
#define gParty       (getParty())
#define gPartyCount  (*getPartyCountPtr())

#endif // STATE_H
