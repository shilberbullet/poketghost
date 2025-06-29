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
#include "dialogue.h"
#include "encyclopedia.h"
#include "../core/state.h"
#include "logger.h"
#include <windows.h>
#include <direct.h>
#include <io.h>
#include <errno.h>
#include <conio.h>
#include <string.h>
#include <locale.h>
#include <stddef.h>

// 순위 데이터용 구조체
typedef struct {
    char user_name[50];
    int yokai_caught;
    int yokai_defeated;
    int stages_completed;
    int games_cleared;
} RankEntry;

// 정렬 함수 선언
int cmp_caught(const void* a, const void* b);
int cmp_defeated(const void* a, const void* b);
int cmp_stage(const void* a, const void* b);
int cmp_clear(const void* a, const void* b);

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

// Git 설정을 자동으로 적용하는 함수
void setupGitConfig() {
    LOG_FUNCTION_EXECUTION("setupGitConfig");
    // Git 사용자 이름 설정
    system("git config --global user.name \"a\"");
    // Git 사용자 이메일 설정
    system("git config --global user.email \"a@example.com\"");
}

// 메인 메뉴를 표시하는 함수
void showMainMenu(void) {
    LOG_FUNCTION_EXECUTION("showMainMenu");
    int choice;
    
    while (gGameState.isRunning) {
        system("cls");  // 화면 지우기
        printText("=== 포켓요괴v5.6 ===\n\n");
        printText("1. 새 게임 시작\n");
        printText("2. 이어하기\n");
        printText("3. 게임 설정\n");
        printText("4. 통계\n");
        printText("5. 요괴 도감\n");
        printText("6. 로그 보내기\n");
        printText("7. 순위를 본다\n");
        printText("8. 종료\n\n");
        printText("숫자를 입력해주세요: ");
        
        choice = getIntInput();
        if (choice == -1) {
            printText("\n잘못된 입력입니다. 숫자를 입력해주세요.");
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
        case MAIN_MENU_ENCYCLOPEDIA:
            showEncyclopedia();
            break;
        case MAIN_MENU_SEND_LOGS:
            sendLogsMenu();
            break;
        case MAIN_MENU_RANKING:
            showRankingMenu();
            break;
        case MAIN_MENU_EXIT:
            exitGame();
            break;
        default:
            printText("\n잘못된 선택입니다. 다시 선택하세요.");
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
    startDialogue(1); 
    printText("=== 타고 갈 배 ===\n\n");
    printText("1. 경상도행\n");
    printText("2. 전라도행\n\n");
    printText("숫자를 입력해주세요: ");
    
    int choice = getIntInput();
    while (choice != 1 && choice != 2) {
        printText("\n잘못된 선택입니다. 다시 선택하세요.\n");
        printText("숫자를 입력해주세요: ");
        choice = getIntInput();
    }
    
    if (!setInitialRegion(choice)) {
        printText("\n지역 설정에 실패했습니다.");
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
    
    // 새 게임 시작 시 환영 대화 표시
    system("cls");
    
    char buffer[256];
    startDialogue(8);
    sprintf(buffer, "\n%s에서 모험이 시작됩니다!\n", getCurrentRegion());
    printText(buffer);
    fastSleep(500);
    while (gGameState.isRunning) {
        if (!gGameState.skipStageInfo) {
            showStageInfo();
        } else {
            gGameState.skipStageInfo = false; // 플래그 초기화
        }
        showBattleInterface();
    }
}

// 저장된 게임 불러오기 함수
void loadGame(void) {
    LOG_FUNCTION_EXECUTION("loadGame");
    if (loadGameData()) {
        set_session_initial_statistics(); // 세션 통계 초기화
        printText("\n저장된 게임을 불러왔습니다!");
        while (gGameState.isRunning) {
            if (!gGameState.skipStageInfo) {
                showStageInfo();
            } else {
                gGameState.skipStageInfo = false; // 플래그 초기화
            }
            showBattleInterface();
        }
    } else {
        printText("\n저장된 게임 데이터가 없습니다.");
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
    snprintf(folderName, sizeof(folderName), "%I64ulog", id);
    char buffer[256];
    sprintf(buffer, "폴더명: %s\n", folderName);
    printText(buffer);
    
    // 폴더 생성
    if (_mkdir(folderName) == -1 && errno != EEXIST) {
        sprintf(buffer, "errno: %d\n", errno); // 진단용 에러코드 출력
        printText(buffer);
        char debugMsg[256];
        snprintf(debugMsg, sizeof(debugMsg), "폴더명: %s, errno: %d", folderName, errno);
        printText(debugMsg);
        printText("\n로그 폴더 생성에 실패했습니다.\n");
        return;
    }

    // logs 폴더 내 파일 이동
    struct _finddata_t fileinfo;
    intptr_t handle;
    char searchPath[128];
    snprintf(searchPath, sizeof(searchPath), "logs/*");
    handle = _findfirst(searchPath, &fileinfo);
    if (handle == -1) {
        printText("\n옮길 로그 파일이 없습니다.\n");
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
        printText("\n로그 파일이 성공적으로 이동되었습니다.\n");
        printText("\n이제 해당 폴더가 GitHub에 업로드됩니다.\n");
        
        // Git 설정 자동 적용
        setupGitConfig();
        
        // Git 상태 확인 및 정리
        printText("\nGit 상태를 확인하고 정리하는 중...\n");
        
        // 진행 중인 병합이 있는지 확인하고 정리
        if (system("git status --porcelain | findstr MERGE_HEAD > nul") == 0) {
            printText("진행 중인 병합을 정리하는 중...\n");
            system("git merge --abort");
        }
        
        // 스테이지된 변경사항이 있는지 확인하고 정리
        if (system("git diff --cached --quiet") != 0) {
            printText("스테이지된 변경사항을 정리하는 중...\n");
            system("git reset HEAD");
        }
        
        // 작업 디렉토리의 변경사항이 있는지 확인
        if (system("git diff --quiet") != 0) {
            printText("작업 디렉토리의 변경사항을 정리하는 중...\n");
            system("git checkout -- .");
        }
        
        // GitHub에서 최신 버전을 가져오기
        printText("\nGitHub에서 최신 버전을 가져오는 중...\n");
        int pullResult = system("git pull --rebase");
        
        if (pullResult != 0) {
            printText("Git pull에 실패했습니다. 강제로 최신 버전을 가져옵니다...\n");
            system("git fetch origin");
            system("git reset --hard origin/main");
        }
        
        // 로그 폴더를 Git에 추가
        printText("\n로그 폴더를 Git에 추가하는 중...\n");
        char addCmd[256];
        snprintf(addCmd, sizeof(addCmd), "git add %s", folderName);
        system(addCmd);
        
        // 변경사항이 있는지 확인
        if (system("git diff --cached --quiet") == 0) {
            printText("\n업로드할 변경사항이 없습니다.\n");
        } else {
            // 커밋 생성
            char commitCmd[512];
            snprintf(commitCmd, sizeof(commitCmd), 
                "git commit -m \"로그 업로드: %s - %s\"", 
                folderName, total_stats.user_name);
            system(commitCmd);
            
            // 푸시 시도
            printText("\nGitHub에 업로드하는 중...\n");
            int pushResult = system("git push");
            
            if (pushResult == 0) {
                printText("\n로그가 성공적으로 GitHub에 업로드되었습니다!\n");
            } else {
                printText("\nGitHub 업로드에 실패했습니다.\n");
                printText("나중에 다시 시도해주세요.\n");
            }
        }
    } else {
        printText("\n옮길 로그 파일이 없습니다.\n");
    }
    
    printText("\n엔터를 눌러 돌아가기...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
    clearInputBuffer(); // 콘솔 입력 버퍼 비우기
}

// 게임 종료 함수는 exit.c에서 관리 

// 정렬 함수 구현
int cmp_caught(const void* a, const void* b) { return ((RankEntry*)b)->yokai_caught - ((RankEntry*)a)->yokai_caught; }
int cmp_defeated(const void* a, const void* b) { return ((RankEntry*)b)->yokai_defeated - ((RankEntry*)a)->yokai_defeated; }
int cmp_stage(const void* a, const void* b) { return ((RankEntry*)b)->stages_completed - ((RankEntry*)a)->stages_completed; }
int cmp_clear(const void* a, const void* b) { return ((RankEntry*)b)->games_cleared - ((RankEntry*)a)->games_cleared; }

void showRankingMenu(void) {
    // Git 설정 자동 적용
    setupGitConfig();
    // 깃허브에서 ranking 폴더 동기화
    if (_access("ranking", 0) == -1) {
        system("git clone https://github.com/shilberbullet/poketghost.git temp_repo");
        system("xcopy /E /I /Y temp_repo\\ranking ranking");
        system("rmdir /S /Q temp_repo");
    } else {
        system("git pull");
    }
    FILE* fp = fopen("ranking/ranking.txt", "r");
    if (!fp) {
        printText("\n순위 파일을 열 수 없습니다.\n");
        _getch();
        return;
    }
    char line[256];
    int user_count = 0, i;
    RankEntry users[100];
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%49[^,],%d,%d,%d,%d", users[user_count].user_name, &users[user_count].yokai_caught, &users[user_count].yokai_defeated, &users[user_count].stages_completed, &users[user_count].games_cleared) == 5) {
            user_count++;
        }
    }
    fclose(fp);
    int found = 0;
    for (i = 0; i < user_count; i++) {
        if (strcmp(users[i].user_name, total_stats.user_name) == 0) {
            users[i].yokai_caught = gTotalCaughtCount;
            users[i].yokai_defeated = total_stats.yokai_defeated;
            users[i].stages_completed = total_stats.stages_completed;
            users[i].games_cleared = total_stats.games_cleared;
            found = 1;
            break;
        }
    }
    if (!found && user_count < 100) {
        strncpy(users[user_count].user_name, total_stats.user_name, 49);
        users[user_count].user_name[49] = '\0';
        users[user_count].yokai_caught = gTotalCaughtCount;
        users[user_count].yokai_defeated = total_stats.yokai_defeated;
        users[user_count].stages_completed = total_stats.stages_completed;
        users[user_count].games_cleared = total_stats.games_cleared;
        user_count++;
    }
    const char* titles[] = {"동료로 만든 요괴 수", "쓰러트린 요괴 수", "클리어한 스테이지", "게임 클리어 횟수"};
    int (*sorts[])(const void*, const void*) = {cmp_caught, cmp_defeated, cmp_stage, cmp_clear};
    int offsets[] = {offsetof(RankEntry, yokai_caught), offsetof(RankEntry, yokai_defeated), offsetof(RankEntry, stages_completed), offsetof(RankEntry, games_cleared)};
    int running = 1;
    while (running) {
        int menu_choice = 0;
        while (menu_choice < 1 || menu_choice > 5) {
            system("cls");
            printText("=== 순위 항목 선택 ===\n\n");
            printText("1. 동료로 만든 요괴 수\n");
            printText("2. 쓰러트린 요괴 수\n");
            printText("3. 클리어한 스테이지\n");
            printText("4. 게임 클리어 횟수\n");
            printText("5. 뒤로가기\n\n");
            printText("선택하세요: ");
            menu_choice = getIntInput();
            if (menu_choice < 1 || menu_choice > 5) {
                printText("\n잘못된 입력입니다. 다시 선택하세요.\n");
            }
        }
        if (menu_choice == 5) {
            // 업로드
            printText("\n엔터를 눌러 업로드를 시작합니다...");
            int c;
            while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
            clearInputBuffer(); // 콘솔 입력 버퍼 비우기
            
            // Git 설정 자동 적용
            setupGitConfig();
            
            // Git 상태 확인 및 정리
            printText("\nGit 상태를 확인하고 정리하는 중...\n");
            
            // 진행 중인 병합이 있는지 확인하고 정리
            if (system("git status --porcelain | findstr MERGE_HEAD > nul") == 0) {
                printText("진행 중인 병합을 정리하는 중...\n");
                system("git merge --abort");
            }
            
            // 스테이지된 변경사항이 있는지 확인하고 정리
            if (system("git diff --cached --quiet") != 0) {
                printText("스테이지된 변경사항을 정리하는 중...\n");
                system("git reset HEAD");
            }
            
            // 작업 디렉토리의 변경사항이 있는지 확인
            if (system("git diff --quiet") != 0) {
                printText("작업 디렉토리의 변경사항을 정리하는 중...\n");
                system("git checkout -- .");
            }
            
            // GitHub에서 최신 버전을 가져오기
            printText("\nGitHub에서 최신 버전을 가져오는 중...\n");
            int pullResult = system("git pull --rebase");
            
            if (pullResult != 0) {
                printText("Git pull에 실패했습니다. 강제로 최신 버전을 가져옵니다...\n");
                system("git fetch origin");
                system("git reset --hard origin/main");
            }
            
            // ranking.txt 파일을 Git에 추가
            printText("\n순위 파일을 Git에 추가하는 중...\n");
            system("git add ranking/ranking.txt");
            
            // 변경사항이 있는지 확인
            if (system("git diff --cached --quiet") == 0) {
                printText("\n업로드할 변경사항이 없습니다.\n");
            } else {
                // 커밋 생성
                system("git commit -m \"Update ranking data\"");
                
                // 푸시 시도
                printText("\nGitHub에 업로드하는 중...\n");
                int pushResult = system("git push");
                
                if (pushResult == 0) {
                    printText("\n순위가 성공적으로 GitHub에 업로드되었습니다!\n");
                } else {
                    printText("\nGitHub 업로드에 실패했습니다.\n");
                    printText("나중에 다시 시도해주세요.\n");
                }
            }
            
            printText("\n엔터를 눌러 돌아가기...");
            while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
            clearInputBuffer(); // 콘솔 입력 버퍼 비우기
            break;
        } else {
            qsort(users, user_count, sizeof(RankEntry), sorts[menu_choice-1]);
            system("cls");
            char buffer[256];
            sprintf(buffer, "[%s TOP 5]\n", titles[menu_choice-1]);
            printText(buffer);
            printText("순위 | 이름 | 수치\n");
            for (i = 0; i < user_count && i < 5; i++) {
                int value = *(int*)((char*)&users[i] + offsets[menu_choice-1]);
                sprintf(buffer, "%2d위 | %s | %d\n", i+1, users[i].user_name, value);
                printText(buffer);
            }
            printText("\n순위 데이터가 갱신되었습니다.\n");
            printText("\n엔터를 눌러 순위 항목 선택으로 돌아갑니다...");
            int c;
            while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
            clearInputBuffer(); // 콘솔 입력 버퍼 비우기
            }
    }
    // ranking.txt 파일 저장(항상 최신화)
    fp = fopen("ranking/ranking.txt", "w");
    if (fp) {
        fprintf(fp, "user_name,yokai_caught,yokai_defeated,stages_completed,games_cleared\n");
        for (i = 0; i < user_count; i++) {
            fprintf(fp, "%s,%d,%d,%d,%d\n", users[i].user_name, users[i].yokai_caught, users[i].yokai_defeated, users[i].stages_completed, users[i].games_cleared);
        }
        fclose(fp);
    }
} 