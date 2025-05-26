#include <stdio.h>
#include <stdlib.h>
#include "settings.h"
#include "settings_file.h"

// 전역 게임 설정 초기화
GameSettings gameSettings = {3};  // 기본값: 보통 속도

void initSettings() {
    loadSettings();  // 시작할 때 설정 파일 불러오기
} 