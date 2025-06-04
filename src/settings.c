// 표준 입출력 함수를 위한 헤더
#include <stdio.h>
// 메모리 할당, 랜덤 함수 등을 위한 헤더
#include <stdlib.h>
// 설정 관련 함수와 구조체 정의
#include "settings.h"
// 설정 파일 관련 함수
#include "settings_file.h"

// 전역 게임 설정 초기화 (기본값: 보통 속도, 한 글자씩 출력)
GameSettings gameSettings = {3, 0, 1};  // textSpeed = 3 (보통 속도), textDisplayMode = 0 (한 글자씩), showTypeHint = 1 (상성 힌트 켬)

// 게임 설정 초기화 함수
void initSettings() {
    loadSettings();  // 시작할 때 설정 파일에서 설정 불러오기
} 