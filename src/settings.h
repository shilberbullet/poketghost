#ifndef SETTINGS_H
#define SETTINGS_H

// 게임 설정 구조체
typedef struct {
    int textSpeed;  // 텍스트 출력 속도 (1: 느림, 2: 보통, 3: 빠름)
} GameSettings;

// 전역 게임 설정
extern GameSettings gameSettings;

// 설정 초기화 함수
void initSettings();

// 설정 메뉴 표시 함수
void showSettingsMenu();

// 텍스트 속도 설정 함수
void setTextSpeed();

// 설정 파일 저장 함수
void saveSettings();

// 설정 파일 불러오기 함수
void loadSettings();

#endif // SETTINGS_H 