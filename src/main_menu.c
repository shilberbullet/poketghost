// 표준 입출력, 게임 및 메뉴 관련 헤더
#include <stdio.h>
#include <stdlib.h>
#include "main_menu.h"
#include "game.h"
#include "input.h"
#include "settings.h"
#include "settings_ui.h"
#include "text.h"
#include "stage.h"
#include "party.h"
#include "savefile.h"
#include "exit.h"
#include "reward.h"
#include "region.h"
#include "statistics.h"
#include "../core/state.h"
#include "logger.h"
#include <windows.h>
#include <direct.h>
#include <io.h>
#include <errno.h>
#include <conio.h>

// 폴더명에서 개행, 공백, 특수문자 제거 함수
void sanitizeFolderName(const char* src, char* dest, size_t destSize) {
    size_t j = 0;
    for (size_t i = 0; src[i] != '\0' && j < destSize - 1; i++) {
        unsigned char c = (unsigned char)src[i];
        // 한글(UTF-8 3바이트), 영문, 숫자만 허용
        if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            dest[j++] = c;
        } else if ((c & 0xE0) == 0xC0 || (c & 0xF0) == 0xE0) { // UTF-8 2/3바이트(한글)
            int len = (c & 0xE0) == 0xC0 ? 2 : 3;
            for (int k = 0; k < len && src[i] && j < destSize - 1; k++) {
                dest[j++] = src[i++];
            }
            i--; // for문에서 1 증가하므로 보정
        }
        // 공백, 개행, 특수문자 등은 무시
    }
    dest[j] = '\0';
}

// 문자열로부터 고유 숫자 ID 생성 함수 (단순 해시)
unsigned long long makeIdFromName(const char* name) {
    unsigned long long hash = 5381;
    int c;
    while ((c = *name++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

// 메인 메뉴를 표시하는 함수
void showMainMenu(void) {
    LOG_FUNCTION_EXECUTION("showMainMenu");
    int choice;
    
    while (gGameState.isRunning) {
        system("cls");  // 화면 지우기
        printText("=== 포켓요괴v3.0 ===\n\n");
        printText("1. 새 게임 시작\n");
        printText("2. 이어하기\n");
        printText("3. 게임 설정\n");
        printText("4. 통계\n");
        printText("5. 로그 보내기\n");
        printText("6. 종료\n\n");
        printText("숫자를 입력해주세요: ");
        
        choice = getIntInput();
        if (choice == -1) {
            printTextAndWait("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            continue;
        }
        
        handleMainMenuChoice((MainMenuOption)choice);
    }
}

// 메인 메뉴 선택을 처리하는 함수
void handleMainMenuChoice(MainMenuOption choice) {
    LOG_FUNCTION_EXECUTION("handleMainMenuChoice");
    switch (choice) {
        case MAIN_MENU_NEW_GAME:
            startNewGame();
            break;
        case MAIN_MENU_LOAD_GAME:
            loadGame();
            break;
        case MAIN_MENU_SETTINGS:
            showSettings();
            break;
        case MAIN_MENU_STATISTICS:
            display_statistics_screen();
            break;
        case MAIN_MENU_SEND_LOGS:
            sendLogsMenu();
            break;
        case MAIN_MENU_EXIT:
            exitGame();
            break;
        default:
            printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");
            break;
    }
}

// 새 게임 시작 함수
void startNewGame(void) {
    LOG_FUNCTION_EXECUTION("startNewGame");
    gGameState.isNewGame = 1;  // 새 게임 플래그 설정
    gGameState.isLoadedGame = 0;  // 이어하기 플래그 해제
    resetItemRewardSystem(); // 아이템 보상 시스템 상태 초기화
    init_player_statistics(); // 플레이어 통계 초기화
    
    // 시작 지역 선택
    system("cls");
    printText("=== 시작 지역 선택 ===\n\n");
    printText("1. 경상도\n");
    printText("2. 전라도\n\n");
    printText("숫자를 입력해주세요: ");
    
    int choice = getIntInput();
    while (choice != 1 && choice != 2) {
        printText("\n잘못된 선택입니다. 다시 선택하세요.\n");
        printText("숫자를 입력해주세요: ");
        choice = getIntInput();
    }
    
    if (!setInitialRegion(choice)) {
        printTextAndWait("\n지역 설정에 실패했습니다.");
        return;
    }
    
    initGame(); // 게임 상태 초기화

    // 게임 시작 시 기본 아이템인 '낡은 부적' 5개를 인벤토리에 추가
    Item* oldTalisman = NULL;
    for (int i = 0; i < itemListCount; i++) {
        if (strcmp(itemList[i].name, "낡은부적") == 0) {
            oldTalisman = &itemList[i];
            break;
        }
    }
    if (oldTalisman) {
        for (int i = 0; i < 5; i++) {
            addItemToInventoryWithoutMessage(oldTalisman);
        }
    }
    
    initStage(&gStage, 1);  // 첫 번째 스테이지로 시작
    initParty();
    
    char buffer[256];
    sprintf(buffer, "\n%s에서 모험이 시작됩니다!\n", getCurrentRegion());
    printTextAndWait(buffer);
    fastSleep(500);
    while (gGameState.isRunning) {
        showStageInfo();
        showBattleInterface();
    }
}

// 저장된 게임 불러오기 함수
void loadGame(void) {
    LOG_FUNCTION_EXECUTION("loadGame");
    if (loadGameData()) {
        set_session_initial_statistics(); // 세션 통계 초기화
        printTextAndWait("\n저장된 게임을 불러왔습니다!");
        while (gGameState.isRunning) {
            showStageInfo();
            showBattleInterface();
        }
    } else {
        printTextAndWait("\n저장된 게임 데이터가 없습니다.");
    }
}

// 설정 메뉴 표시 함수
void showSettings(void) {
    LOG_FUNCTION_EXECUTION("showSettings");
    // 기존 설정 메뉴 표시 함수 호출
    showSettingsMenu();
}

// 로그 보내기 함수 구현
void sendLogsMenu(void) {
    LOG_FUNCTION_EXECUTION("sendLogsMenu");
    char sanitizedName[64];
    sanitizeFolderName(total_stats.user_name, sanitizedName, sizeof(sanitizedName));
    unsigned long long id = makeIdFromName(sanitizedName);
    char folderName[100];
    snprintf(folderName, sizeof(folderName), "%llulog", id);
    printf("폴더명: %s\n", folderName);
    if (_mkdir(folderName) == -1 && errno != EEXIST) {
        printf("errno: %d\n", errno); // 진단용 에러코드 출력
        char debugMsg[256];
        snprintf(debugMsg, sizeof(debugMsg), "폴더명: %s, errno: %d", folderName, errno);
        printTextAndWait(debugMsg);
        printTextAndWait("\n로그 폴더 생성에 실패했습니다.\n");
        return;
    }

    // logs 폴더 내 파일 이동
    struct _finddata_t fileinfo;
    intptr_t handle;
    char searchPath[128];
    snprintf(searchPath, sizeof(searchPath), "logs/*");
    handle = _findfirst(searchPath, &fileinfo);
    if (handle == -1) {
        printTextAndWait("\n옮길 로그 파일이 없습니다.\n");
        return;
    }
    int moved = 0;
    do {
        if (!(fileinfo.attrib & _A_SUBDIR)) {
            char srcPath[256], destPath[256];
            snprintf(srcPath, sizeof(srcPath), "logs/%s", fileinfo.name);
            snprintf(destPath, sizeof(destPath), "%s/%s", folderName, fileinfo.name);
            if (rename(srcPath, destPath) == 0) {
                moved++;
            }
        }
    } while (_findnext(handle, &fileinfo) == 0);
    _findclose(handle);

    if (moved > 0) {
        printTextAndWait("\n로그 파일이 성공적으로 이동되었습니다.\n");
        printTextAndWait("\n이제 해당 폴더가 GitHub에 업로드됩니다.\n");
        // GitHub 업로드: 해당 폴더만 add/commit/push
        char gitCmd[512];
        snprintf(gitCmd, sizeof(gitCmd),
            "git add %s && git commit -m \"로그 업로드: %s\" && git push",
            folderName, folderName);
        system(gitCmd);
    } else {
        printTextAndWait("\n옮길 로그 파일이 없습니다.\n");
    }
    printTextAndWait("\n아무 키나 누르면 메인 메뉴로 돌아갑니다...\n");
    _getch();
}

// 게임 종료 함수는 exit.c에서 관리 