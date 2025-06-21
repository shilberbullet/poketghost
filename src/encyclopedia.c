#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encyclopedia.h"
#include "input.h"
#include "text.h"
#include "yokai.h"
#include "savefile.h"
#include "statistics.h"
#include "../core/state.h"
#include "logger.h"

// 잡은 요괴 정보를 저장할 전역 변수
static int caughtYokai[200] = {0}; // 0: 안잡음, 1: 잡음 (일반 + 보스 + 패러독스)
#define totalYokaiCount (yokaiListCount + bossYokaiListCount + paradoxYokaiListCount)

// 요괴 인덱스로부터 실제 요괴를 가져오는 함수
Yokai* getYokaiByIndex(int index) {
    if (index < 0) return NULL;
    
    if (index < yokaiListCount) {
        return &yokaiList[index];
    } else if (index < yokaiListCount + bossYokaiListCount) {
        return &bossYokaiList[index - yokaiListCount];
    } else if (index < yokaiListCount + bossYokaiListCount + paradoxYokaiListCount) {
        return &paradoxYokaiList[index - yokaiListCount - bossYokaiListCount];
    }
    
    return NULL;
}

// 도감 표시 함수
void showEncyclopedia(void) {
    LOG_FUNCTION_EXECUTION("showEncyclopedia");
    int choice;
    
    while (1) {
        system("cls");
        printText("=== 요괴 도감 ===\n\n");
        printText("1. 전체 요괴 보기\n");
        printText("2. 타입별 보기\n");
        printText("3. 잡은 요괴 보기\n");
        printText("4. 돌아가기\n\n");
        printText("숫자를 입력해주세요: ");
        
        choice = getIntInput();
        if (choice == -1) {
            printText("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            continue;
        }
        
        if (handleEncyclopediaChoice((EncyclopediaOption)choice)) {
            break; // 돌아가기 선택 시 루프 종료
        }
    }
}

// 도감 메뉴 선택 처리 함수
int handleEncyclopediaChoice(EncyclopediaOption choice) {
    LOG_FUNCTION_EXECUTION("handleEncyclopediaChoice");
    switch (choice) {
        case ENCYCLOPEDIA_ALL:
            showAllYokai();
            break;
        case ENCYCLOPEDIA_BY_TYPE:
            showYokaiByType();
            break;
        case ENCYCLOPEDIA_CAUGHT:
            showCaughtYokai();
            break;
        case ENCYCLOPEDIA_BACK:
            return 1; // 돌아가기
        default:
            printText("\n잘못된 선택입니다. 다시 선택하세요.");
            break;
    }
    return 0;
}

// 전체 요괴 목록 표시 함수
void showAllYokai(void) {
    LOG_FUNCTION_EXECUTION("showAllYokai");
    int page = 0;
    int yokaiPerPage = 20;
    int totalPages = (totalYokaiCount + yokaiPerPage - 1) / yokaiPerPage;
    
    while (1) {
        system("cls");
        printText("=== 전체 요괴 목록 ===\n\n");
        
        int startIndex = page * yokaiPerPage;
        int endIndex = (startIndex + yokaiPerPage < totalYokaiCount) ? 
                      startIndex + yokaiPerPage : totalYokaiCount;
        
        for (int i = startIndex; i < endIndex; i++) {
            Yokai* yokai = getYokaiByIndex(i);
            if (yokai) {
                char buffer[256];
                const char* caughtStatus = isYokaiCaught(i + 1) ? "✓" : "?";
                sprintf(buffer, "%3d. %s %s\n", i + 1, caughtStatus, yokai->name);
                printText(buffer);
            }
        }
        
        char pageBuffer[256];
        sprintf(pageBuffer, "\n=== 페이지 %d/%d ===\n", page + 1, totalPages);
        printText(pageBuffer);
        printText("1. 이전 페이지\n");
        printText("2. 다음 페이지\n");
        printText("3. 요괴 상세보기\n");
        printText("4. 돌아가기\n\n");
        printText("숫자를 입력해주세요: ");
        
        int choice = getIntInput();
        if (choice == -1) {
            printText("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            continue;
        }
        
        {
            char inputBuffer[256];
            int yokaiChoice;
            
            switch (choice) {
                case 1: // 이전 페이지
                    if (page > 0) page--;
                    break;
                case 2: // 다음 페이지
                    if (page < totalPages - 1) page++;
                    break;
                case 3: // 요괴 상세보기
                    sprintf(inputBuffer, "\n요괴 번호를 입력하세요 (1-%d): ", totalYokaiCount);
                    printText(inputBuffer);
                    yokaiChoice = getIntInput();
                    if (yokaiChoice >= 1 && yokaiChoice <= totalYokaiCount) {
                        showYokaiDetail(yokaiChoice - 1);
                    } else {
                        printText("\n잘못된 번호입니다.");
                    }
                    break;
                case 4: // 돌아가기
                    return;
                default:
                    printText("\n잘못된 선택입니다. 다시 선택하세요.");
                    break;
            }
        }
    }
}

// 타입별 요괴 목록 표시 함수
void showYokaiByType(void) {
    LOG_FUNCTION_EXECUTION("showYokaiByType");
    int choice;
    
    while (1) {
        system("cls");
        printText("=== 타입별 요괴 보기 ===\n\n");
        printText("1. 전체 타입\n");
        printText("2. 괴수 (MONSTER)\n");
        printText("3. 귀신 (GHOST)\n");
        printText("4. 동물 (ANIMAL)\n");
        printText("5. 인간 (HUMAN)\n");
        printText("6. 악령 (EVIL_SPIRIT)\n");
        printText("7. 돌아가기\n\n");
        printText("숫자를 입력해주세요: ");
        
        choice = getIntInput();
        if (choice == -1) {
            printText("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            continue;
        }
        
        if (choice == 7) break; // 돌아가기
        
        if (choice >= 1 && choice <= 6) {
            showYokaiBySpecificType(choice - 1);
        } else {
            printText("\n잘못된 선택입니다. 다시 선택하세요.");
        }
    }
}

// 특정 타입의 요괴 목록 표시 함수
void showYokaiBySpecificType(int typeFilter) {
    LOG_FUNCTION_EXECUTION("showYokaiBySpecificType");
    int page = 0;
    int yokaiPerPage = 20;
    
    // 해당 타입의 요괴 수 계산
    int typeCount = 0;
    for (int i = 0; i < totalYokaiCount; i++) {
        Yokai* yokai = getYokaiByIndex(i);
        if (yokai && (typeFilter == 0 || (int)yokai->type == typeFilter - 1)) {
            typeCount++;
        }
    }
    
    int totalPages = (typeCount + yokaiPerPage - 1) / yokaiPerPage;
    
    while (1) {
        system("cls");
        const char* typeName = getTypeNameInKorean(typeFilter - 1);
        char buffer[256];
        sprintf(buffer, "=== %s 타입 요괴 목록 ===\n\n", typeName);
        printText(buffer);
        
        int displayed = 0;
        int startIndex = page * yokaiPerPage;
        int currentIndex = 0;
        
        for (int i = 0; i < totalYokaiCount && displayed < yokaiPerPage; i++) {
            Yokai* yokai = getYokaiByIndex(i);
            if (yokai && (typeFilter == 0 || (int)yokai->type == typeFilter - 1)) {
                if (currentIndex >= startIndex) {
                    char yokaiBuffer[256];
                    const char* caughtStatus = isYokaiCaught(i + 1) ? "✓" : "?";
                    sprintf(yokaiBuffer, "%3d. %s %s\n", i + 1, caughtStatus, yokai->name);
                    printText(yokaiBuffer);
                    displayed++;
                }
                currentIndex++;
            }
        }
        
        if (totalPages > 1) {
            char pageBuffer[256];
            sprintf(pageBuffer, "\n=== 페이지 %d/%d ===\n", page + 1, totalPages);
            printText(pageBuffer);
        }
        
        printText("1. 이전 페이지\n");
        printText("2. 다음 페이지\n");
        printText("3. 요괴 상세보기\n");
        printText("4. 돌아가기\n\n");
        printText("숫자를 입력해주세요: ");
        
        int choice = getIntInput();
        if (choice == -1) {
            printText("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            continue;
        }
        
        {
            char inputBuffer[256];
            int yokaiChoice;
            
            switch (choice) {
                case 1: // 이전 페이지
                    if (page > 0) page--;
                    break;
                case 2: // 다음 페이지
                    if (page < totalPages - 1) page++;
                    break;
                case 3: // 요괴 상세보기
                    sprintf(inputBuffer, "\n요괴 번호를 입력하세요 (1-%d): ", totalYokaiCount);
                    printText(inputBuffer);
                    yokaiChoice = getIntInput();
                    if (yokaiChoice >= 1 && yokaiChoice <= totalYokaiCount) {
                        showYokaiDetail(yokaiChoice - 1);
                    } else {
                        printText("\n잘못된 번호입니다.");
                    }
                    break;
                case 4: // 돌아가기
                    return;
                default:
                    printText("\n잘못된 선택입니다. 다시 선택하세요.");
                    break;
            }
        }
    }
}

// 잡은 요괴 목록 표시 함수
void showCaughtYokai(void) {
    LOG_FUNCTION_EXECUTION("showCaughtYokai");
    int caughtCount = getCaughtYokaiCount();
    
    if (caughtCount == 0) {
        system("cls");
        printText("=== 잡은 요괴 목록 ===\n\n");
        printText("아직 잡은 요괴가 없습니다.\n");
        printText("\n엔터를 눌러 돌아가기...");
        int c;
        while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
        clearInputBuffer(); // 콘솔 입력 버퍼 비우기      
        return;
    }
    
    int page = 0;
    int yokaiPerPage = 20;
    int totalPages = (caughtCount + yokaiPerPage - 1) / yokaiPerPage;
    
    while (1) {
        system("cls");
        printText("=== 잡은 요괴 목록 ===\n\n");
        
        int displayed = 0;
        int startIndex = page * yokaiPerPage;
        int currentIndex = 0;
        
        for (int i = 0; i < totalYokaiCount && displayed < yokaiPerPage; i++) {
            if (isYokaiCaught(i + 1)) {
                Yokai* yokai = getYokaiByIndex(i);
                if (yokai) {
                    if (currentIndex >= startIndex) {
                        char buffer[256];
                        sprintf(buffer, "%3d. ✓ %s\n", i + 1, yokai->name);
                        printText(buffer);
                        displayed++;
                    }
                    currentIndex++;
                }
            }
        }
        
        char buffer[256];
        sprintf(buffer, "\n=== 페이지 %d/%d (총 %d마리 잡음) ===\n", page + 1, totalPages, caughtCount);
        printText(buffer);
        printText("1. 이전 페이지\n");
        printText("2. 다음 페이지\n");
        printText("3. 요괴 상세보기\n");
        printText("4. 돌아가기\n\n");
        printText("숫자를 입력해주세요: ");
        
        int choice = getIntInput();
        if (choice == -1) {
            printText("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            continue;
        }
        
        {
            char inputBuffer[256];
            int yokaiChoice;
            
            switch (choice) {
                case 1: // 이전 페이지
                    if (page > 0) page--;
                    break;
                case 2: // 다음 페이지
                    if (page < totalPages - 1) page++;
                    break;
                case 3: // 요괴 상세보기
                    sprintf(inputBuffer, "\n요괴 번호를 입력하세요 (1-%d): ", totalYokaiCount);
                    printText(inputBuffer);
                    yokaiChoice = getIntInput();
                    if (yokaiChoice >= 1 && yokaiChoice <= totalYokaiCount) {
                        showYokaiDetail(yokaiChoice - 1);
                    } else {
                        printText("\n잘못된 번호입니다.");
                    }
                    break;
                case 4: // 돌아가기
                    return;
                default:
                    printText("\n잘못된 선택입니다. 다시 선택하세요.");
                    break;
            }
        }
    }
}

// 요괴 상세 정보 표시 함수
void showYokaiDetail(int yokaiIndex) {
    LOG_FUNCTION_EXECUTION("showYokaiDetail");
    if (yokaiIndex < 0 || yokaiIndex >= totalYokaiCount) {
        printText("\n잘못된 요괴 번호입니다.");
        return;
    }
    
    Yokai* yokai = getYokaiByIndex(yokaiIndex);
    if (!yokai) {
        printText("\n요괴를 찾을 수 없습니다.");
        return;
    }
    
    system("cls");
    printText("=== 요괴 상세 정보 ===\n\n");
    
    char buffer[512];
    
    sprintf(buffer, "번호: %d\n", yokaiIndex + 1);
    printText(buffer);
    
    sprintf(buffer, "이름: %s\n", yokai->name);
    printText(buffer);
    
    const char* typeName = getTypeNameInKorean(yokai->type);
    sprintf(buffer, "타입: %s\n", typeName);
    printText(buffer);
    
    sprintf(buffer, "공격력: %d\n", yokai->attack);
    printText(buffer);
    
    sprintf(buffer, "방어력: %d\n", yokai->defense);
    printText(buffer);
    
    sprintf(buffer, "체력: %d\n", yokai->stamina);
    printText(buffer);
    
    sprintf(buffer, "스피드: %d\n", yokai->speed);
    printText(buffer);
    
    sprintf(buffer, "설명: %s\n", yokai->desc);
    printText(buffer);
    
    const char* caughtStatus = isYokaiCaught(yokaiIndex + 1) ? "잡음" : "안잡음";
    sprintf(buffer, "상태: %s\n", caughtStatus);
    printText(buffer);
    
    printText("\n배울 수 있는 기술:\n");
    for (int i = 0; i < yokai->learnableMoveCount; i++) {
        const char* colorCode;
        switch (yokai->learnableMoves[i].type) {
            case TYPE_EVIL_SPIRIT:
                colorCode = "\033[31m";  // 빨간색
                break;
            case TYPE_GHOST:
                colorCode = "\033[35m";  // 보라색
                break;
            case TYPE_MONSTER:
                colorCode = "\033[33m";  // 노란색
                break;
            case TYPE_HUMAN:
                colorCode = "\033[36m";  // 청록색
                break;
            case TYPE_ANIMAL:
                colorCode = "\033[32m";  // 초록색
                break;
            default:
                colorCode = "\033[0m";   // 기본색
        }
        sprintf(buffer, "%d. %s%s\033[0m\n", i + 1, colorCode, yokai->learnableMoves[i].name);
        printText(buffer);
    }
    
    printText("\n엔터를 눌러 돌아가기...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // 표준 입력 버퍼 비우기
    clearInputBuffer(); // 콘솔 입력 버퍼 비우기
}

// 타입 이름을 한글로 변환하는 함수
const char* getTypeNameInKorean(YokaiType type) {
    switch (type) {
        case TYPE_MONSTER:
            return "괴수";
        case TYPE_GHOST:
            return "귀신";
        case TYPE_ANIMAL:
            return "동물";
        case TYPE_HUMAN:
            return "인간";
        case TYPE_EVIL_SPIRIT:
            return "악령";
        default:
            return "알 수 없음";
    }
}

// 요괴를 잡았는지 확인하는 함수
int isYokaiCaught(int yokaiIndex) {
    if (yokaiIndex < 1 || yokaiIndex > totalYokaiCount) {
        return 0;
    }
    return caughtYokai[yokaiIndex - 1];
}

// 잡은 요괴 수를 반환하는 함수
int getCaughtYokaiCount(void) {
    int count = 0;
    for (int i = 0; i < totalYokaiCount; i++) {
        if (caughtYokai[i]) {
            count++;
        }
    }
    return count;
}

// 요괴를 잡은 것으로 표시하는 함수 (다른 모듈에서 호출)
void markYokaiAsCaught(int yokaiIndex) {
    if (yokaiIndex >= 1 && yokaiIndex <= totalYokaiCount) {
        // 이미 잡은 요괴인지 확인
        if (caughtYokai[yokaiIndex - 1] == 0) {
            caughtYokai[yokaiIndex - 1] = 1;
            // 즉시 저장하지 않음 - 자동저장이나 수동저장 시에만 저장
        }
    }
}

// 잡은 요괴 정보를 초기화하는 함수
void resetCaughtYokai(void) {
    memset(caughtYokai, 0, sizeof(caughtYokai));
    
    // 도깨비를 찾아서 잡은 상태로 설정
    for (int i = 0; i < totalYokaiCount; i++) {
        Yokai* yokai = getYokaiByIndex(i);
        if (yokai && strcmp(yokai->name, "도깨비") == 0) {
            caughtYokai[i] = 1;
            break;
        }
    }
}

// 잡은 요괴 정보를 파일에 저장하는 함수
void saveCaughtYokaiData(void) {
    FILE* file = fopen("data/caught_yokai.dat", "wb");
    if (file) {
        fwrite(caughtYokai, sizeof(caughtYokai), 1, file);
        fclose(file);
    }
}

// 잡은 요괴 정보를 파일에서 로드하는 함수
void loadCaughtYokaiData(void) {
    FILE* file = fopen("data/caught_yokai.dat", "rb");
    if (file) {
        fread(caughtYokai, sizeof(caughtYokai), 1, file);
        fclose(file);
    } else {
        // 파일이 없으면 도깨비만 잡은 상태로 초기화
        resetCaughtYokai();
    }
} 