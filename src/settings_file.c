// 표준 입출력 함수를 위한 헤더
#include <stdio.h> // 표준 입출력 함수
// 메모리 할당, 랜덤 함수 등을 위한 헤더
#include <stdlib.h> // 표준 라이브러리 함수
// 설정 관련 함수와 구조체 정의
#include "settings.h" // 설정 관련 헤더
// 설정 파일 관련 함수
#include "settings_file.h" // 설정 파일 관련 헤더
// 로깅 함수를 위한 헤더
#include "logger.h" // 로거 헤더

// 설정 파일 경로 정의
#define SETTINGS_FILE "data/settings.dat" // 설정 파일 경로 상수

// 설정 파일 저장 함수
void saveSettings() { // 설정 파일 저장 함수 시작
    LOG_FUNCTION_EXECUTION("saveSettings"); // 함수 실행 로그 기록
    FILE* file = fopen(SETTINGS_FILE, "wb");  // 바이너리 쓰기 모드로 파일 열기
    if (file != NULL) { // 파일이 성공적으로 열렸으면
        fwrite(&gameSettings, sizeof(GameSettings), 1, file);  // 게임 설정을 파일에 저장
        fclose(file);  // 파일 닫기
    }
}

// 설정 파일 불러오기 함수
void loadSettings() { // 설정 파일 불러오기 함수 시작
    LOG_FUNCTION_EXECUTION("loadSettings"); // 함수 실행 로그 기록
    FILE* file = fopen(SETTINGS_FILE, "rb");  // 바이너리 읽기 모드로 파일 열기
    if (file != NULL) { // 파일이 성공적으로 열렸으면
        fread(&gameSettings, sizeof(GameSettings), 1, file);  // 파일에서 게임 설정을 불러옴
        fclose(file);  // 파일 닫기
    }
} 