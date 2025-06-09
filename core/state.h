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

// 매크로로 기존 전역 변수 이름을 제공
#define gameState (*getGameState())
#define player (*getPlayer())
#define currentStage (*getCurrentStage())
#define party (getParty())
#define partyCount (*getPartyCountPtr())

#endif // STATE_H
