#include "state.h"

static GameState _gameState = {0};
static Player _player = {0};
static StageInfo _currentStage = {0};
static Yokai _party[MAX_PARTY_SIZE];
static int _partyCount = 0;

GameState* getGameState() {
    return &_gameState;
}

void setGameState(GameState state) {
    _gameState = state;
}

Player* getPlayer() {
    return &_player;
}

void setPlayer(Player player) {
    _player = player;
}

StageInfo* getCurrentStage() {
    return &_currentStage;
}

void setCurrentStage(StageInfo stage) {
    _currentStage = stage;
}

Yokai* getParty() {
    return _party;
}

int* getPartyCountPtr() {
    return &_partyCount;
}

void setPartyCount(int count) {
    _partyCount = count;
}
