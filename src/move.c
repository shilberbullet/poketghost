#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "move.h"
#include "text.h"

const char* typeNames[] = {
    "악귀", "원귀", "괴수형", "인간형", "동물형"
};

Move moveList[MAX_MOVES_TOTAL];
int moveListCount = 0;

void loadMovesFromFile(const char* filename) {
    moveListCount = 0;
    FILE* file = fopen(filename, "r");
    if (!file) return;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        char* name = strtok(line, ",");
        char* type = strtok(NULL, ",");
        char* power = strtok(NULL, ",");
        char* acc = strtok(NULL, ",");
        char* pp = strtok(NULL, ",");
        char* desc = strtok(NULL, "\n");
        if (name && type && power && acc && pp && desc && moveListCount < MAX_MOVES_TOTAL) {
            strncpy(moveList[moveListCount].name, name, YOKAI_NAME_MAX);
            moveList[moveListCount].type = parseType(type);
            moveList[moveListCount].power = atoi(power);
            moveList[moveListCount].accuracy = atoi(acc);
            moveList[moveListCount].pp = atoi(pp);
            strncpy(moveList[moveListCount].description, desc, 255);
            moveListCount++;
        }
    }
    fclose(file);
}

Move* findMoveByName(const char* name) {
    for (int i = 0; i < moveListCount; i++) {
        if (strcmp(moveList[i].name, name) == 0) return &moveList[i];
    }
    return NULL;
}

void printMoveInfo(const Move* move) {
    printf("\n=== %s ===\n", move->name);
    printf("공격력: %d\n", move->power);
    printf("명중률: %d%%\n", move->accuracy);
    printf("설명: %s\n", move->description);
}

YokaiType parseType(const char* typeStr) {
    for (int i = 0; i < TYPE_COUNT; i++) {
        if (strcmp(typeStr, typeNames[i]) == 0) return (YokaiType)i;
    }
    return TYPE_EVIL_SPIRIT;
} 