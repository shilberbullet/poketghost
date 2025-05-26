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

#endif // SETTINGS_H 