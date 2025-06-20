// 표준 입출력 함수를 위한 헤더
#include <stdio.h>
// 메모리 할당, 랜덤 함수 등을 위한 헤더
#include <stdlib.h>
// 설정 관련 함수와 구조체 정의
#include "settings.h"
// 설정 파일 관련 함수
#include "settings_file.h"
// 로깅 함수를 위한 헤더
#include "logger.h"

// 설정 파일 경로 정의
#define SETTINGS_FILE "data/settings.dat"

// 설정 파일 저장 함수
void saveSettings() {
    LOG_FUNCTION_EXECUTION("saveSettings");
    FILE* file = fopen(SETTINGS_FILE, "wb");  // 바이너리 쓰기 모드로 파일 열기
    if (file != NULL) {
        fwrite(&gameSettings, sizeof(GameSettings), 1, file);  // 게임 설정을 파일에 저장
        fclose(file);  // 파일 닫기
    }
}

// 설정 파일 불러오기 함수
void loadSettings() {
    LOG_FUNCTION_EXECUTION("loadSettings");
    FILE* file = fopen(SETTINGS_FILE, "rb");  // 바이너리 읽기 모드로 파일 열기
    if (file != NULL) {
        fread(&gameSettings, sizeof(GameSettings), 1, file);  // 파일에서 게임 설정을 불러옴
        fclose(file);  // 파일 닫기
    }
} 