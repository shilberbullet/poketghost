// 표준 입출력, 설정 관련 헤더
#include <stdio.h> // 표준 입출력 함수
#include <stdlib.h> // 표준 라이브러리 함수
#include "settings.h" // 설정 관련 헤더
#include "settings_ui.h" // 설정 UI 관련 헤더
#include "settings_file.h" // 설정 파일 관련 헤더
#include "input.h" // 입력 처리 헤더
#include "text.h" // 텍스트 출력 헤더
#include "logger.h" // 로거 헤더

// 설정 메뉴를 표시하는 함수
void showSettingsMenu() { // 설정 메뉴 표시 함수 시작
    LOG_FUNCTION_EXECUTION("showSettingsMenu"); // 함수 실행 로그 기록
    int choice; // 사용자 선택 변수
    
    while (1) { // 무한 루프 시작
        system("cls"); // 화면 지우기
        printText("=== 게임 설정 ===\n\n"); // 제목 출력
        printText("1. 게임 출력 속도\n"); // 메뉴 항목 1 출력
        printText("2. 텍스트 출력 방식\n"); // 메뉴 항목 2 출력
        printText("3. 상성 힌트 표시: "); // 메뉴 항목 3 시작
        printText(gameSettings.showTypeHint ? "켬\n" : "끔\n"); // 현재 상태 출력
        printText("4. 디버그 모드: "); // 메뉴 항목 4 시작
        printText(gameSettings.debugMode ? "켬\n" : "끔\n"); // 현재 상태 출력
        printText("5. 대화 시스템: "); // 메뉴 항목 5 시작
        printText(gameSettings.dialogueEnabled ? "켬\n" : "끔\n"); // 현재 상태 출력
        printText("6. 뒤로 가기\n\n"); // 뒤로가기 옵션 출력
        printText("숫자를 입력해주세요: "); // 입력 안내 메시지 출력
        
        choice = getIntInput(); // 사용자 입력 받기
        if (choice == -1) { // 잘못된 입력인 경우
            printText("\n잘못된 선택입니다. 다시 선택하세요."); // 오류 메시지 출력
            continue; // 루프 처음으로 돌아가기
        }
        
        switch (choice) { // 사용자 선택에 따른 처리
            case 1: // 게임 출력 속도 설정
                setTextSpeed(); // 텍스트 속도 설정 함수 호출
                break; // 케이스 종료
            case 2: // 텍스트 출력 방식 설정
                setTextDisplayMode(); // 텍스트 출력 방식 설정 함수 호출
                break; // 케이스 종료
            case 3: // 상성 힌트 표시 설정
                setTypeHintDisplay(); // 상성 힌트 표시 설정 함수 호출
                break; // 케이스 종료
            case 4: // 디버그 모드 설정
                setDebugMode(); // 디버그 모드 설정 함수 호출
                break; // 케이스 종료
            case 5: // 대화 시스템 설정
                setDialogueEnabled(); // 대화 시스템 설정 함수 호출
                break; // 케이스 종료
            case 6: // 뒤로 가기
                return; // 함수 종료
            default: // 잘못된 선택
                printText("\n잘못된 선택입니다. 다시 선택하세요."); // 오류 메시지 출력
                break; // 케이스 종료
        }
    }
}

// 텍스트 출력 속도 설정 함수
void setTextSpeed() { // 텍스트 출력 속도 설정 함수 시작
    LOG_FUNCTION_EXECUTION("setTextSpeed"); // 함수 실행 로그 기록
    int choice; // 사용자 선택 변수
    
    while (1) { // 무한 루프 시작
        system("cls"); // 화면 지우기
        printText("=== 게임 출력 속도 설정 ===\n\n"); // 제목 출력
        
        // 현재 속도 표시
        printText("현재 속도: "); // 현재 속도 안내
        printText(gameSettings.textSpeed == 1 ? "매우 느림" : // 속도 1인 경우
                 gameSettings.textSpeed == 2 ? "느림" : // 속도 2인 경우
                 gameSettings.textSpeed == 3 ? "보통" : // 속도 3인 경우
                 gameSettings.textSpeed == 4 ? "빠름" : "매우 빠름"); // 속도 4인 경우 또는 5인 경우
        printText("\n\n"); // 줄바꿈
        
        printText("1. 매우 느림\n"); // 옵션 1 출력
        printText("2. 느림\n"); // 옵션 2 출력
        printText("3. 보통\n"); // 옵션 3 출력
        printText("4. 빠름\n"); // 옵션 4 출력
        printText("5. 매우 빠름\n"); // 옵션 5 출력
        printText("6. 뒤로 가기\n\n"); // 뒤로가기 옵션 출력
        printText("선택하세요 (1-6): "); // 입력 안내 메시지 출력
        
        choice = getIntInput(); // 사용자 입력 받기
        if (choice == -1) { // 잘못된 입력인 경우
            printText("\n잘못된 선택입니다. 다시 선택하세요."); // 오류 메시지 출력
            continue; // 루프 처음으로 돌아가기
        }
        
        switch (choice) { // 사용자 선택에 따른 처리
            case 1: // 매우 느림
            case 2: // 느림
            case 3: // 보통
            case 4: // 빠름
            case 5: // 매우 빠름
                gameSettings.textSpeed = choice; // 설정 변경
                saveSettings();  // 설정 변경 시 파일에 저장
                printText("\n게임 출력 속도가 변경되었습니다."); // 완료 메시지 출력
                return; // 함수 종료
            case 6: // 뒤로 가기
                return; // 함수 종료
            default: // 잘못된 선택
                printText("\n잘못된 선택입니다. 다시 선택하세요."); // 오류 메시지 출력
                break; // 케이스 종료
        }
    }
}

// 텍스트 출력 방식 설정 함수
void setTextDisplayMode() { // 텍스트 출력 방식 설정 함수 시작
    LOG_FUNCTION_EXECUTION("setTextDisplayMode"); // 함수 실행 로그 기록
    int choice; // 사용자 선택 변수
    
    while (1) { // 무한 루프 시작
        system("cls"); // 화면 지우기
        printText("=== 텍스트 출력 방식 설정 ===\n\n"); // 제목 출력
        
        // 현재 출력 방식 표시
        printText("현재 출력 방식: "); // 현재 방식 안내
        printText(gameSettings.textDisplayMode == 0 ? "한 글자씩" : // 방식 0인 경우
                 gameSettings.textDisplayMode == 1 ? "한 줄씩" : "한 번에"); // 방식 1인 경우 또는 2인 경우
        printText("\n\n"); // 줄바꿈
        
        printText("1. 한 글자씩 출력\n"); // 옵션 1 출력
        printText("2. 한 줄씩 출력\n"); // 옵션 2 출력
        printText("3. 한 번에 출력\n"); // 옵션 3 출력
        printText("4. 뒤로 가기\n\n"); // 뒤로가기 옵션 출력
        printText("선택하세요 (1-4): "); // 입력 안내 메시지 출력
        
        choice = getIntInput(); // 사용자 입력 받기
        if (choice == -1) { // 잘못된 입력인 경우
            printText("\n잘못된 선택입니다. 다시 선택하세요."); // 오류 메시지 출력
            continue; // 루프 처음으로 돌아가기
        }
        
        switch (choice) { // 사용자 선택에 따른 처리
            case 1: // 한 글자씩 출력
                gameSettings.textDisplayMode = 0; // 설정 변경
                saveSettings();  // 설정 변경 시 파일에 저장
                printText("\n텍스트 출력 방식이 '한 글자씩'으로 변경되었습니다."); // 완료 메시지 출력
                return; // 함수 종료
            case 2: // 한 줄씩 출력
                gameSettings.textDisplayMode = 1; // 설정 변경
                saveSettings();  // 설정 변경 시 파일에 저장
                printText("\n텍스트 출력 방식이 '한 줄씩'으로 변경되었습니다."); // 완료 메시지 출력
                return; // 함수 종료
            case 3: // 한 번에 출력
                gameSettings.textDisplayMode = 2; // 설정 변경
                saveSettings();  // 설정 변경 시 파일에 저장
                printText("\n텍스트 출력 방식이 '한 번에'로 변경되었습니다."); // 완료 메시지 출력
                return; // 함수 종료
            case 4: // 뒤로 가기
                return; // 함수 종료
            default: // 잘못된 선택
                printText("\n잘못된 선택입니다. 다시 선택하세요."); // 오류 메시지 출력
                break; // 케이스 종료
        }
    }
}

// 상성 힌트 표시 설정 함수
void setTypeHintDisplay() { // 상성 힌트 표시 설정 함수 시작
    LOG_FUNCTION_EXECUTION("setTypeHintDisplay"); // 함수 실행 로그 기록
    system("cls"); // 화면 지우기
    printText("=== 상성 힌트 표시 설정 ===\n\n"); // 제목 출력
    printText("현재 상태: "); // 현재 상태 안내
    printText(gameSettings.showTypeHint ? "켬\n\n" : "끔\n\n"); // 현재 상태 출력
    printText("1. 켬\n"); // 옵션 1 출력
    printText("2. 끔\n"); // 옵션 2 출력
    printText("3. 뒤로 가기\n\n"); // 뒤로가기 옵션 출력
    printText("선택하세요 (1-3): "); // 입력 안내 메시지 출력
    int choice = getIntInput(); // 사용자 입력 받기
    switch (choice) { // 사용자 선택에 따른 처리
        case 1: // 켬
            gameSettings.showTypeHint = 1; // 설정 변경
            saveSettings(); // 설정 저장
            printText("\n상성 힌트가 표시됩니다."); // 완료 메시지 출력
            break; // 케이스 종료
        case 2: // 끔
            gameSettings.showTypeHint = 0; // 설정 변경
            saveSettings(); // 설정 저장
            printText("\n상성 힌트가 표시되지 않습니다."); // 완료 메시지 출력
            break; // 케이스 종료
        case 3: // 뒤로 가기
            return; // 함수 종료
        default: // 잘못된 선택
            printText("\n잘못된 선택입니다. 다시 선택하세요."); // 오류 메시지 출력
            setTypeHintDisplay(); // 함수 재귀 호출
            break; // 케이스 종료
    }
}

void setDebugMode() { // 디버그 모드 설정 함수 시작
    LOG_FUNCTION_EXECUTION("setDebugMode"); // 함수 실행 로그 기록
    system("cls"); // 화면 지우기
    printText("=== 디버그 모드 설정 ===\n\n"); // 제목 출력
    printText("현재 상태: "); // 현재 상태 안내
    printText(gameSettings.debugMode ? "켬\n\n" : "끔\n\n"); // 현재 상태 출력
    printText("1. 켬\n"); // 옵션 1 출력
    printText("2. 끔\n"); // 옵션 2 출력
    printText("3. 뒤로 가기\n\n"); // 뒤로가기 옵션 출력
    printText("선택하세요 (1-3): "); // 입력 안내 메시지 출력
    int choice = getIntInput(); // 사용자 입력 받기
    switch (choice) { // 사용자 선택에 따른 처리
        case 1: // 켬
            gameSettings.debugMode = 1; // 설정 변경
            saveSettings(); // 설정 저장
            printText("\n디버그 모드가 켜졌습니다."); // 완료 메시지 출력
            break; // 케이스 종료
        case 2: // 끔
            gameSettings.debugMode = 0; // 설정 변경
            saveSettings(); // 설정 저장
            printText("\n디버그 모드가 꺼졌습니다."); // 완료 메시지 출력
            break; // 케이스 종료
        case 3: // 뒤로 가기
            return; // 함수 종료
        default: // 잘못된 선택
            printText("\n잘못된 선택입니다. 다시 선택하세요."); // 오류 메시지 출력
            setDebugMode(); // 함수 재귀 호출
            break; // 케이스 종료
    }
}

void setDialogueEnabled() { // 대화 시스템 설정 함수 시작
    LOG_FUNCTION_EXECUTION("setDialogueEnabled"); // 함수 실행 로그 기록
    system("cls"); // 화면 지우기
    printText("=== 대화 시스템 설정 ===\n\n"); // 제목 출력
    printText("현재 상태: "); // 현재 상태 안내
    printText(gameSettings.dialogueEnabled ? "켬\n\n" : "끔\n\n"); // 현재 상태 출력
    printText("1. 켬\n"); // 옵션 1 출력
    printText("2. 끔\n"); // 옵션 2 출력
    printText("3. 뒤로 가기\n\n"); // 뒤로가기 옵션 출력
    printText("선택하세요 (1-3): "); // 입력 안내 메시지 출력
    int choice = getIntInput(); // 사용자 입력 받기
    switch (choice) { // 사용자 선택에 따른 처리
        case 1: // 켬
            gameSettings.dialogueEnabled = 1; // 설정 변경
            saveSettings(); // 설정 저장
            printText("\n대화 시스템이 켜졌습니다."); // 완료 메시지 출력
            break; // 케이스 종료
        case 2: // 끔
            gameSettings.dialogueEnabled = 0; // 설정 변경
            saveSettings(); // 설정 저장
            printText("\n대화 시스템이 꺼졌습니다."); // 완료 메시지 출력
            break; // 케이스 종료
        case 3: // 뒤로 가기
            return; // 함수 종료
        default: // 잘못된 선택
            printText("\n잘못된 선택입니다. 다시 선택하세요."); // 오류 메시지 출력
            setDialogueEnabled(); // 함수 재귀 호출
            break; // 케이스 종료
    }
} 