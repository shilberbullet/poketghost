#include <stdio.h>
#include <stdlib.h>
#include "settings.h"
#include "settings_file.h"

#define SETTINGS_FILE "data/settings.dat"

// 설정 파일 저장 함수
void saveSettings() {
    FILE* file = fopen(SETTINGS_FILE, "wb");
    if (file != NULL) {
        fwrite(&gameSettings, sizeof(GameSettings), 1, file);
        fclose(file);
    }
}

// 설정 파일 불러오기 함수
void loadSettings() {
    FILE* file = fopen(SETTINGS_FILE, "rb");
    if (file != NULL) {
        fread(&gameSettings, sizeof(GameSettings), 1, file);
        fclose(file);
    }
} 