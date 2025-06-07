#ifndef SAVEFILE_H
#define SAVEFILE_H

#include "game.h"
#include "region.h"

void saveGame();
int loadGameData();
void removeSaveFile();
void saveRegionData(FILE* file);
void loadRegionData(FILE* file);

#endif // SAVEFILE_H 