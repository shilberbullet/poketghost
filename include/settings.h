#ifndef SETTINGS_H
#define SETTINGS_H

// 게임 설정 구조체
typedef struct {
    int textSpeed;     // 텍스트 출력 속도 (1: 매우 느림, 2: 느림, 3: 보통, 4: 빠름, 5: 매우 빠름)
    int textDisplayMode;  // 텍스트 출력 방식 (0: 한 글자씩, 1: 한 줄씩)
    int showTypeHint; // 상성 힌트 표시 여부 (0: 끔, 1: 켬)
    int debugMode;    // 디버그 모드 여부 (0: 끔, 1: 켬)
    int dialogueEnabled; // 대화 시스템 켜기/끄기 (0: 끔, 1: 켬)
} GameSettings;

// 전역 게임 설정
extern GameSettings gameSettings;

// 설정 초기화 함수
void initSettings();

#endif // SETTINGS_H 