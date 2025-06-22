#include <stdio.h>  // 표준 입출력 함수 헤더 포함
#include <stdlib.h>  // 표준 라이브러리 함수 헤더 포함
#include <string.h>  // 문자열 처리 함수 헤더 포함
#include "encyclopedia.h"  // 도감 시스템 헤더 포함
#include "input.h"  // 입력 처리 헤더 포함
#include "text.h"  // 텍스트 출력 헤더 포함
#include "yokai.h"  // 요괴 시스템 헤더 포함
#include "savefile.h"  // 저장 파일 헤더 포함
#include "statistics.h"  // 통계 시스템 헤더 포함
#include "../core/state.h"  // 게임 상태 헤더 포함
#include "logger.h"  // 로깅 시스템 헤더 포함

// 잡은 요괴 정보를 저장할 전역 변수
static int caughtYokai[200] = {0};  // 0: 안잡음, 1: 잡음 (일반 + 보스 + 패러독스)
#define totalYokaiCount (yokaiListCount + bossYokaiListCount + paradoxYokaiListCount)  // 전체 요괴 수 계산

// 요괴 인덱스로부터 실제 요괴를 가져오는 함수
Yokai* getYokaiByIndex(int index) {  // 인덱스로 요괴 포인터 반환
    if (index < 0) return NULL;  // 음수 인덱스는 NULL 반환
    
    if (index < yokaiListCount) {  // 일반 요괴 범위
        return &yokaiList[index];  // 일반 요괴 반환
    } else if (index < yokaiListCount + bossYokaiListCount) {  // 보스 요괴 범위
        return &bossYokaiList[index - yokaiListCount];  // 보스 요괴 반환
    } else if (index < yokaiListCount + bossYokaiListCount + paradoxYokaiListCount) {  // 패러독스 요괴 범위
        return &paradoxYokaiList[index - yokaiListCount - bossYokaiListCount];  // 패러독스 요괴 반환
    }
    
    return NULL;  // 범위 밖이면 NULL 반환
}

// 도감 표시 함수
void showEncyclopedia(void) {  // 도감 메인 화면 표시 함수
    LOG_FUNCTION_EXECUTION("showEncyclopedia");  // 함수 실행 로그 기록
    int choice;  // 사용자 선택 변수
    
    while (1) {  // 무한 루프
        system("cls");  // 화면 지우기
        printText("=== 요괴 도감 ===\n\n");  // 제목 출력
        printText("1. 전체 요괴 보기\n");  // 메뉴 출력
        printText("2. 타입별 보기\n");
        printText("3. 잡은 요괴 보기\n");
        printText("4. 돌아가기\n\n");
        printText("숫자를 입력해주세요: ");  // 입력 안내
        
        choice = getIntInput();  // 사용자 입력 받기
        if (choice == -1) {  // 잘못된 입력
            printText("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            continue;  // 다시 입력
        }
        
        if (handleEncyclopediaChoice((EncyclopediaOption)choice)) {  // 메뉴 선택 처리
            break;  // 돌아가기 선택 시 루프 종료
        }
    }
}

// 도감 메뉴 선택 처리 함수
int handleEncyclopediaChoice(EncyclopediaOption choice) {  // 도감 메뉴 선택 처리 함수
    LOG_FUNCTION_EXECUTION("handleEncyclopediaChoice");  // 함수 실행 로그 기록
    switch (choice) {  // 선택 분기
        case ENCYCLOPEDIA_ALL:
            showAllYokai();  // 전체 요괴 보기
            break;
        case ENCYCLOPEDIA_BY_TYPE:
            showYokaiByType();  // 타입별 보기
            break;
        case ENCYCLOPEDIA_CAUGHT:
            showCaughtYokai();  // 잡은 요괴 보기
            break;
        case ENCYCLOPEDIA_BACK:
            return 1;  // 돌아가기
        default:
            printText("\n잘못된 선택입니다. 다시 선택하세요.");
            break;
    }
    return 0;  // 계속 루프
}

// 전체 요괴 목록 표시 함수
void showAllYokai(void) {  // 전체 요괴 목록 표시 함수
    LOG_FUNCTION_EXECUTION("showAllYokai");  // 함수 실행 로그 기록
    int page = 0;  // 현재 페이지
    int yokaiPerPage = 20;  // 한 페이지당 요괴 수
    int totalPages = (totalYokaiCount + yokaiPerPage - 1) / yokaiPerPage;  // 전체 페이지 수 계산
    
    while (1) {  // 무한 루프
        system("cls");  // 화면 지우기
        printText("=== 전체 요괴 목록 ===\n\n");  // 제목 출력
        
        int startIndex = page * yokaiPerPage;  // 시작 인덱스 계산
        int endIndex = (startIndex + yokaiPerPage < totalYokaiCount) ? 
                      startIndex + yokaiPerPage : totalYokaiCount;  // 끝 인덱스 계산
        
        for (int i = startIndex; i < endIndex; i++) {  // 현재 페이지의 요괴 출력
            Yokai* yokai = getYokaiByIndex(i);  // 요괴 포인터 가져오기
            if (yokai) {
                char buffer[256];  // 출력용 버퍼
                const char* caughtStatus = isYokaiCaught(i + 1) ? "✓" : "?";  // 잡은 여부
                sprintf(buffer, "%3d. %s %s\n", i + 1, caughtStatus, yokai->name);  // 요괴 정보 포맷팅
                printText(buffer);  // 출력
            }
        }
        
        char pageBuffer[256];  // 페이지 정보 버퍼
        sprintf(pageBuffer, "\n=== 페이지 %d/%d ===\n", page + 1, totalPages);  // 페이지 정보 포맷팅
        printText(pageBuffer);  // 페이지 정보 출력
        printText("1. 이전 페이지\n");
        printText("2. 다음 페이지\n");
        printText("3. 요괴 상세보기\n");
        printText("4. 돌아가기\n\n");
        printText("숫자를 입력해주세요: ");  // 입력 안내
        
        int choice = getIntInput();  // 사용자 입력 받기
        if (choice == -1) {  // 잘못된 입력
            printText("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            continue;  // 다시 입력
        }
        
        {
            char inputBuffer[256];  // 입력 버퍼
            int yokaiChoice;  // 요괴 선택 번호
            
            switch (choice) {
                case 1:  // 이전 페이지
                    if (page > 0) page--;
                    break;
                case 2:  // 다음 페이지
                    if (page < totalPages - 1) page++;
                    break;
                case 3:  // 요괴 상세보기
                    sprintf(inputBuffer, "\n요괴 번호를 입력하세요 (1-%d): ", totalYokaiCount);
                    printText(inputBuffer);
                    yokaiChoice = getIntInput();
                    if (yokaiChoice >= 1 && yokaiChoice <= totalYokaiCount) {
                        showYokaiDetail(yokaiChoice - 1);  // 상세 정보 표시
                    } else {
                        printText("\n잘못된 번호입니다.");
                    }
                    break;
                case 4:  // 돌아가기
                    return;
                default:
                    printText("\n잘못된 선택입니다. 다시 선택하세요.");
                    break;
            }
        }
    }
}

// 타입별 요괴 목록 표시 함수
void showYokaiByType(void) {  // 타입별 요괴 목록 표시 함수
    LOG_FUNCTION_EXECUTION("showYokaiByType");  // 함수 실행 로그 기록
    int choice;  // 사용자 선택 변수
    
    while (1) {  // 무한 루프
        system("cls");  // 화면 지우기
        printText("=== 타입별 요괴 보기 ===\n\n");  // 제목 출력
        printText("1. 전체 타입\n");
        printText("2. 괴수 (MONSTER)\n");
        printText("3. 귀신 (GHOST)\n");
        printText("4. 동물 (ANIMAL)\n");
        printText("5. 인간 (HUMAN)\n");
        printText("6. 악령 (EVIL_SPIRIT)\n");
        printText("7. 돌아가기\n\n");
        printText("숫자를 입력해주세요: ");  // 입력 안내
        
        choice = getIntInput();  // 사용자 입력 받기
        if (choice == -1) {  // 잘못된 입력
            printText("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            continue;  // 다시 입력
        }
        
        if (choice == 7) break;  // 돌아가기
        
        if (choice >= 1 && choice <= 6) {
            showYokaiBySpecificType(choice - 1);  // 타입별 요괴 목록 표시
        } else {
            printText("\n잘못된 선택입니다. 다시 선택하세요.");
        }
    }
}

// 특정 타입의 요괴 목록 표시 함수
void showYokaiBySpecificType(int typeFilter) {  // 특정 타입 요괴 목록 표시 함수
    LOG_FUNCTION_EXECUTION("showYokaiBySpecificType");  // 함수 실행 로그 기록
    int page = 0;  // 현재 페이지
    int yokaiPerPage = 20;  // 한 페이지당 요괴 수
    
    // 해당 타입의 요괴 수 계산
    int typeCount = 0;  // 타입별 요괴 수
    for (int i = 0; i < totalYokaiCount; i++) {
        Yokai* yokai = getYokaiByIndex(i);  // 요괴 포인터 가져오기
        if (yokai && (typeFilter == 0 || (int)yokai->type == typeFilter - 1)) {
            typeCount++;  // 해당 타입이면 카운트 증가
        }
    }
    
    int totalPages = (typeCount + yokaiPerPage - 1) / yokaiPerPage;  // 전체 페이지 수 계산
    
    while (1) {  // 무한 루프
        system("cls");  // 화면 지우기
        const char* typeName = getTypeNameInKorean(typeFilter - 1);  // 타입명 한글 변환
        char buffer[256];  // 출력용 버퍼
        sprintf(buffer, "=== %s 타입 요괴 목록 ===\n\n", typeName);  // 제목 포맷팅
        printText(buffer);  // 제목 출력
        
        int displayed = 0;  // 현재 페이지에 표시된 요괴 수
        int startIndex = page * yokaiPerPage;  // 시작 인덱스
        int currentIndex = 0;  // 현재 인덱스
        
        for (int i = 0; i < totalYokaiCount && displayed < yokaiPerPage; i++) {
            Yokai* yokai = getYokaiByIndex(i);  // 요괴 포인터 가져오기
            if (yokai && (typeFilter == 0 || (int)yokai->type == typeFilter - 1)) {
                if (currentIndex >= startIndex) {
                    char yokaiBuffer[256];  // 출력용 버퍼
                    const char* caughtStatus = isYokaiCaught(i + 1) ? "✓" : "?";  // 잡은 여부
                    sprintf(yokaiBuffer, "%3d. %s %s\n", i + 1, caughtStatus, yokai->name);  // 요괴 정보 포맷팅
                    printText(yokaiBuffer);  // 출력
                    displayed++;
                }
                currentIndex++;
            }
        }
        
        if (totalPages > 1) {
            char pageBuffer[256];  // 페이지 정보 버퍼
            sprintf(pageBuffer, "\n=== 페이지 %d/%d ===\n", page + 1, totalPages);  // 페이지 정보 포맷팅
            printText(pageBuffer);  // 페이지 정보 출력
        }
        
        printText("1. 이전 페이지\n");
        printText("2. 다음 페이지\n");
        printText("3. 요괴 상세보기\n");
        printText("4. 돌아가기\n\n");
        printText("숫자를 입력해주세요: ");  // 입력 안내
        
        int choice = getIntInput();  // 사용자 입력 받기
        if (choice == -1) {  // 잘못된 입력
            printText("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            continue;  // 다시 입력
        }
        
        {
            char inputBuffer[256];  // 입력 버퍼
            int yokaiChoice;  // 요괴 선택 번호
            
            switch (choice) {
                case 1:  // 이전 페이지
                    if (page > 0) page--;
                    break;
                case 2:  // 다음 페이지
                    if (page < totalPages - 1) page++;
                    break;
                case 3:  // 요괴 상세보기
                    sprintf(inputBuffer, "\n요괴 번호를 입력하세요 (1-%d): ", totalYokaiCount);
                    printText(inputBuffer);
                    yokaiChoice = getIntInput();
                    if (yokaiChoice >= 1 && yokaiChoice <= totalYokaiCount) {
                        showYokaiDetail(yokaiChoice - 1);  // 상세 정보 표시
                    } else {
                        printText("\n잘못된 번호입니다.");
                    }
                    break;
                case 4:  // 돌아가기
                    return;
                default:
                    printText("\n잘못된 선택입니다. 다시 선택하세요.");
                    break;
            }
        }
    }
}

// 잡은 요괴 목록 표시 함수
void showCaughtYokai(void) {  // 잡은 요괴 목록 표시 함수 정의
    LOG_FUNCTION_EXECUTION("showCaughtYokai");  // 함수 실행 로그 기록
    int caughtCount = getCaughtYokaiCount();  // 잡은 요괴 수 계산
    
    if (caughtCount == 0) {  // 잡은 요괴가 없는 경우
        system("cls");  // 화면 지우기
        printText("=== 잡은 요괴 목록 ===\n\n");  // 제목 출력
        printText("아직 잡은 요괴가 없습니다.\n");  // 안내 메시지 출력
        printText("\n엔터를 눌러 돌아가기...");  // 안내 메시지 출력
        int c;
        while ((c = getchar()) != '\n' && c != EOF);  // 표준 입력 버퍼 비우기
        clearInputBuffer();  // 콘솔 입력 버퍼 비우기
        return;  // 함수 종료
    }
    
    int page = 0;  // 현재 페이지
    int yokaiPerPage = 20;  // 한 페이지당 요괴 수
    int totalPages = (caughtCount + yokaiPerPage - 1) / yokaiPerPage;  // 전체 페이지 수 계산
    
    while (1) {  // 무한 루프
        system("cls");  // 화면 지우기
        printText("=== 잡은 요괴 목록 ===\n\n");  // 제목 출력
        
        int displayed = 0;  // 현재 페이지에 표시된 요괴 수
        int startIndex = page * yokaiPerPage;  // 시작 인덱스
        int currentIndex = 0;  // 현재 인덱스
        
        for (int i = 0; i < totalYokaiCount && displayed < yokaiPerPage; i++) {  // 전체 요괴 순회
            if (isYokaiCaught(i + 1)) {  // 잡은 요괴인 경우
                Yokai* yokai = getYokaiByIndex(i);  // 요괴 포인터 가져오기
                if (yokai) {
                    if (currentIndex >= startIndex) {
                        char buffer[256];  // 출력용 버퍼
                        sprintf(buffer, "%3d. ✓ %s\n", i + 1, yokai->name);  // 요괴 정보 포맷팅
                        printText(buffer);  // 출력
                        displayed++;
                    }
                    currentIndex++;
                }
            }
        }
        
        char buffer[256];  // 페이지 정보 버퍼
        sprintf(buffer, "\n=== 페이지 %d/%d (총 %d마리 잡음) ===\n", page + 1, totalPages, caughtCount);  // 페이지 정보 포맷팅
        printText(buffer);  // 페이지 정보 출력
        printText("1. 이전 페이지\n");
        printText("2. 다음 페이지\n");
        printText("3. 요괴 상세보기\n");
        printText("4. 돌아가기\n\n");
        printText("숫자를 입력해주세요: ");  // 입력 안내
        
        int choice = getIntInput();  // 사용자 입력 받기
        if (choice == -1) {  // 잘못된 입력
            printText("\n잘못된 입력입니다. 숫자를 입력해주세요.");
            continue;  // 다시 입력
        }
        
        {
            char inputBuffer[256];  // 입력 버퍼
            int yokaiChoice;  // 요괴 선택 번호
            
            switch (choice) {
                case 1:  // 이전 페이지
                    if (page > 0) page--;
                    break;
                case 2:  // 다음 페이지
                    if (page < totalPages - 1) page++;
                    break;
                case 3:  // 요괴 상세보기
                    sprintf(inputBuffer, "\n요괴 번호를 입력하세요 (1-%d): ", totalYokaiCount);
                    printText(inputBuffer);
                    yokaiChoice = getIntInput();
                    if (yokaiChoice >= 1 && yokaiChoice <= totalYokaiCount) {
                        showYokaiDetail(yokaiChoice - 1);  // 상세 정보 표시
                    } else {
                        printText("\n잘못된 번호입니다.");
                    }
                    break;
                case 4:  // 돌아가기
                    return;
                default:
                    printText("\n잘못된 선택입니다. 다시 선택하세요.");
                    break;
            }
        }
    }
}

// 요괴 상세 정보 표시 함수
void showYokaiDetail(int yokaiIndex) {  // 요괴 상세 정보 표시 함수 정의
    LOG_FUNCTION_EXECUTION("showYokaiDetail");  // 함수 실행 로그 기록
    if (yokaiIndex < 0 || yokaiIndex >= totalYokaiCount) {  // 유효하지 않은 인덱스
        printText("\n잘못된 요괴 번호입니다.");  // 오류 메시지 출력
        return;  // 함수 종료
    }
    
    Yokai* yokai = getYokaiByIndex(yokaiIndex);  // 요괴 포인터 가져오기
    if (!yokai) {  // 요괴가 없는 경우
        printText("\n요괴를 찾을 수 없습니다.");  // 오류 메시지 출력
        return;  // 함수 종료
    }
    
    system("cls");  // 화면 지우기
    printText("=== 요괴 상세 정보 ===\n\n");  // 제목 출력
    
    char buffer[512];  // 출력용 버퍼
    
    sprintf(buffer, "번호: %d\n", yokaiIndex + 1);  // 번호 출력
    printText(buffer);
    
    sprintf(buffer, "이름: %s\n", yokai->name);  // 이름 출력
    printText(buffer);
    
    const char* typeName = getTypeNameInKorean(yokai->type);  // 타입명 한글 변환
    sprintf(buffer, "타입: %s\n", typeName);  // 타입 출력
    printText(buffer);
    
    sprintf(buffer, "공격력: %d\n", yokai->attack);  // 공격력 출력
    printText(buffer);
    
    sprintf(buffer, "방어력: %d\n", yokai->defense);  // 방어력 출력
    printText(buffer);
    
    sprintf(buffer, "체력: %d\n", yokai->stamina);  // 체력 출력
    printText(buffer);
    
    sprintf(buffer, "스피드: %d\n", yokai->speed);  // 스피드 출력
    printText(buffer);
    
    sprintf(buffer, "설명: %s\n", yokai->desc);  // 설명 출력
    printText(buffer);
    
    const char* caughtStatus = isYokaiCaught(yokaiIndex + 1) ? "잡음" : "안잡음";  // 잡은 상태
    sprintf(buffer, "상태: %s\n", caughtStatus);  // 상태 출력
    printText(buffer);
    
    printText("\n배울 수 있는 기술:\n");  // 기술 목록 안내
    for (int i = 0; i < yokai->learnableMoveCount; i++) {  // 배울 수 있는 기술 순회
        const char* colorCode;  // 색상 코드 변수
        switch (yokai->learnableMoves[i].type) {  // 타입별 색상
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
        sprintf(buffer, "%d. %s%s\033[0m\n", i + 1, colorCode, yokai->learnableMoves[i].name);  // 기술명 출력
        printText(buffer);  // 출력
    }
    
    printText("\n엔터를 눌러 돌아가기...");  // 안내 메시지 출력
    int c;
    while ((c = getchar()) != '\n' && c != EOF);  // 표준 입력 버퍼 비우기
    clearInputBuffer();  // 콘솔 입력 버퍼 비우기
}

// 타입 이름을 한글로 변환하는 함수
const char* getTypeNameInKorean(YokaiType type) {  // 타입 이름 한글 변환 함수 정의
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
int isYokaiCaught(int yokaiIndex) {  // 요괴 잡음 여부 확인 함수 정의
    if (yokaiIndex < 1 || yokaiIndex > totalYokaiCount) {  // 유효하지 않은 인덱스
        return 0;  // 잡지 않음
    }
    return caughtYokai[yokaiIndex - 1];  // 잡은 상태 반환
}

// 잡은 요괴 수를 반환하는 함수
int getCaughtYokaiCount(void) {  // 잡은 요괴 수 반환 함수 정의
    int count = 0;  // 카운트 변수
    for (int i = 0; i < totalYokaiCount; i++) {
        if (caughtYokai[i]) {  // 잡은 경우
            count++;
        }
    }
    return count;  // 총 잡은 수 반환
}

// 요괴를 잡은 것으로 표시하는 함수 (다른 모듈에서 호출)
void markYokaiAsCaught(int yokaiIndex) {  // 요괴 잡음 표시 함수 정의
    if (yokaiIndex >= 1 && yokaiIndex <= totalYokaiCount) {  // 유효한 인덱스
        // 이미 잡은 요괴인지 확인
        if (caughtYokai[yokaiIndex - 1] == 0) {  // 처음 잡은 경우
            caughtYokai[yokaiIndex - 1] = 1;  // 잡음 표시
            // 즉시 저장하지 않음 - 자동저장이나 수동저장 시에만 저장
        }
    }
}

// 잡은 요괴 정보를 초기화하는 함수
void resetCaughtYokai(void) {  // 잡은 요괴 정보 초기화 함수 정의
    memset(caughtYokai, 0, sizeof(caughtYokai));  // 배열 0으로 초기화
    
    // 도깨비를 찾아서 잡은 상태로 설정
    for (int i = 0; i < totalYokaiCount; i++) {
        Yokai* yokai = getYokaiByIndex(i);  // 요괴 포인터 가져오기
        if (yokai && strcmp(yokai->name, "도깨비") == 0) {  // 도깨비인 경우
            caughtYokai[i] = 1;  // 잡음 표시
            break;  // 루프 종료
        }
    }
}

// 잡은 요괴 정보를 파일에 저장하는 함수
void saveCaughtYokaiData(void) {  // 잡은 요괴 정보 저장 함수 정의
    FILE* file = fopen("data/caught_yokai.dat", "wb");  // 파일 열기
    if (file) {
        fwrite(caughtYokai, sizeof(caughtYokai), 1, file);  // 파일에 쓰기
        fclose(file);  // 파일 닫기
    }
}

// 잡은 요괴 정보를 파일에서 로드하는 함수
void loadCaughtYokaiData(void) {  // 잡은 요괴 정보 로드 함수 정의
    FILE* file = fopen("data/caught_yokai.dat", "rb");  // 파일 열기
    if (file) {
        fread(caughtYokai, sizeof(caughtYokai), 1, file);  // 파일에서 읽기
        fclose(file);  // 파일 닫기
    } else {
        // 파일이 없으면 도깨비만 잡은 상태로 초기화
        resetCaughtYokai();  // 도깨비만 잡은 상태로 초기화
    }
} 