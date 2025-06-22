#include "../include/region.h" // 지역 관련 함수와 구조체 정의를 포함하는 헤더 파일
#include "../include/text.h" // 텍스트 출력 함수 정의 헤더 파일
#include "../include/input.h" // 사용자 입력 처리 함수 정의 헤더 파일
#include "../include/settings.h" // 게임 설정 관련 함수 정의 헤더 파일
#include "../include/logger.h" // 함수 실행 로그 기록 함수 정의 헤더 파일
#include "../include/game.h" // 게임 전역 상태 및 관련 함수 정의 헤더 파일
#include "../core/state.h" // 게임 상태 구조체 및 전역 변수 정의 헤더 파일
#include <windows.h> // Windows API 함수 사용을 위한 헤더 파일
#include <stdbool.h>  // bool 타입(참/거짓 논리값) 사용을 위한 헤더 파일
#include <dialogue.h>

// 게임 내 모든 지역 정보를 저장하는 정적 배열, 각 지역은 이름, 방문여부, 연결된 지역 수, 연결된 지역명 배열로 구성됨
static Region regionData[MAX_REGIONS] = {
    {"함경도", 0, 3, {"평안도", "강원도", "황해도"}}, // 함경도: 방문여부 0, 연결 3개
    {"평안도", 0, 2, {"함경도", "황해도"}}, // 평안도: 방문여부 0, 연결 2개
    {"황해도", 0, 3, {"평안도", "경기도", "강원도"}}, // 황해도: 방문여부 0, 연결 3개
    {"강원도", 0, 5, {"함경도", "황해도", "경기도", "경상도", "충청도"}}, // 강원도: 방문여부 0, 연결 5개
    {"경기도", 0, 3, {"황해도", "강원도", "충청도"}}, // 경기도: 방문여부 0, 연결 3개
    {"충청도", 0, 4, {"경기도", "경상도", "전라도", "강원도"}}, // 충청도: 방문여부 0, 연결 4개
    {"경상도", 0, 3, {"강원도", "충청도", "전라도"}}, // 경상도: 방문여부 0, 연결 3개
    {"전라도", 0, 2, {"충청도", "경상도"}} // 전라도: 방문여부 0, 연결 2개
};

// 현재 플레이어가 위치한 지역명을 저장하는 정적 문자열 배열
static char currentRegion[REGION_NAME_LENGTH];

// 지역 시스템을 초기화하는 함수, 랜덤 시드 설정 및 로그 기록
void initRegionSystem(void) {
    LOG_FUNCTION_EXECUTION("initRegionSystem"); // 함수 실행 시 로그 기록
    srand((unsigned int)time(NULL)); // 현재 시간을 시드로 사용하여 난수 생성기 초기화
}

// 현재 지역명을 반환하는 함수, 로그 기록 후 지역명 반환
const char* getCurrentRegion(void) {
    LOG_FUNCTION_EXECUTION("getCurrentRegion"); // 함수 실행 시 로그 기록
    return currentRegion; // 현재 지역명 반환
}

// 플레이어가 선택한 초기 지역을 설정하는 함수, 선택값에 따라 지역명과 방문여부 갱신
int setInitialRegion(int choice) {
    LOG_FUNCTION_EXECUTION("setInitialRegion"); // 함수 실행 시 로그 기록
    if (choice == 1) { // 1번 선택 시
        strcpy(currentRegion, "경상도"); // 현재 지역을 경상도로 설정
        regionData[6].visited = 1;  // 경상도 방문 표시(배열 인덱스 6)
        return 1; // 성공 반환
    } else if (choice == 2) { // 2번 선택 시
        strcpy(currentRegion, "전라도"); // 현재 지역을 전라도로 설정
        regionData[7].visited = 1;  // 전라도 방문 표시(배열 인덱스 7)
        return 1; // 성공 반환
    }
    return 0; // 잘못된 선택 시 실패 반환
}

int moveToNextRegion(void) {
    LOG_FUNCTION_EXECUTION("moveToNextRegion"); // 함수 실행 시 로그 기록
    // 현재 지역의 인덱스를 찾기 위한 변수 초기화
    int currentIndex = -1; // 현재 지역 인덱스(초기값 -1)
    for (int i = 0; i < MAX_REGIONS; i++) { // 모든 지역을 순회하며
        if (strcmp(regionData[i].name, currentRegion) == 0) { // 현재 지역명과 일치하는 지역을 찾으면
            currentIndex = i; // 인덱스 저장
            break; // 반복문 종료
        }
    }
    
    if (currentIndex == -1) return 0; // 현재 지역을 찾지 못하면 실패 반환
    
    // 방문 가능한 지역 인덱스를 저장할 배열과 카운트 변수
    int availableRegions[MAX_REGIONS]; // 방문 가능한 지역 인덱스 배열
    int availableCount = 0; // 방문 가능한 지역 수
    
    // 현재 지역과 연결된 지역 중 방문하지 않은 지역을 찾음
    for (int i = 0; i < regionData[currentIndex].connectedCount; i++) { // 연결된 지역 수만큼 반복
        for (int j = 0; j < MAX_REGIONS; j++) { // 모든 지역을 순회하며
            if (strcmp(regionData[currentIndex].connected[i], regionData[j].name) == 0) { // 연결된 지역명과 일치하는 지역을 찾으면
                if (!regionData[j].visited) {  // 방문하지 않은 지역만 추가
                    availableRegions[availableCount++] = j; // 인덱스 추가 및 카운트 증가
                }
                break; // 내부 반복문 종료
            }
        }
    }
    
    if (availableCount == 0) { // 방문 가능한 지역이 없는 경우
        // 방문하지 않은 지역을 모두 찾기 위한 배열과 카운트 변수
        int unvisitedRegions[MAX_REGIONS]; // 방문하지 않은 지역 인덱스 배열
        int unvisitedCount = 0; // 방문하지 않은 지역 수
        
        for (int i = 0; i < MAX_REGIONS; i++) { // 모든 지역을 순회하며
            if (!regionData[i].visited) { // 방문하지 않은 지역이면
                unvisitedRegions[unvisitedCount++] = i; // 인덱스 추가 및 카운트 증가
            }
        }
        
        if (unvisitedCount > 0) { // 방문하지 않은 지역이 하나라도 있으면
            int cost = gPlayer.money / 2; // 이동 비용(보유전의 50%) 계산
            gPlayer.money -= cost; // 이동 비용 차감
            startDialogue(2000); // 이벤트 대사 출력
            printText("행선지를 알수없는 배를 타고 이동합니다.\n"); // 안내 메시지 출력
            char buffer[128]; // 메시지 버퍼
            sprintf(buffer, "이동 비용으로 %d전을 지불했습니다. (남은 전: %d전)\n", cost, gPlayer.money); // 비용 안내 메시지 생성
            printText(buffer); // 메시지 출력
            fastSleep(1000); // 1초 대기
            
            int nextIndex = unvisitedRegions[rand() % unvisitedCount]; // 방문하지 않은 지역 중 랜덤 선택
            strcpy(currentRegion, regionData[nextIndex].name); // 현재 지역명 갱신
            regionData[nextIndex].visited = 1; // 방문 표시
            return 1; // 성공 반환
        }
        return 0; // 방문하지 않은 지역이 없으면 실패 반환
    }
    
    int nextIndex = availableRegions[rand() % availableCount]; // 방문 가능한 지역 중 랜덤 선택
    strcpy(currentRegion, regionData[nextIndex].name); // 현재 지역명 갱신
    regionData[nextIndex].visited = 1; // 방문 표시
    
    return 1; // 성공 반환
}

// 지도 아이템 사용 시 플레이어가 직접 지역을 선택할 수 있는 함수 정의 시작
int moveToNextRegionWithMap(void) {
    LOG_FUNCTION_EXECUTION("moveToNextRegionWithMap"); // 함수 실행 시 로그 기록
    int currentIndex = -1; // 현재 지역 인덱스(초기값 -1)
    for (int i = 0; i < MAX_REGIONS; i++) { // 모든 지역을 순회하며
        if (strcmp(regionData[i].name, currentRegion) == 0) { // 현재 지역명과 일치하는 지역을 찾으면
            currentIndex = i; // 인덱스 저장
            break; // 반복문 종료
        }
    }
    if (currentIndex == -1) return 0; // 현재 지역을 찾지 못하면 실패 반환
    
    int availableRegions[MAX_REGIONS]; // 인접한 지역 중 방문하지 않은 지역 인덱스 배열
    int availableCount = 0; // 방문하지 않은 인접 지역 수
    
    // 현재 지역과 연결된 모든 지역 확인
    for (int i = 0; i < regionData[currentIndex].connectedCount; i++) { // 연결된 지역 수만큼 반복
        const char* connectedRegion = regionData[currentIndex].connected[i]; // 연결된 지역명 포인터
        for (int j = 0; j < MAX_REGIONS; j++) { // 모든 지역을 순회하며
            if (strcmp(regionData[j].name, connectedRegion) == 0) { // 연결된 지역명과 일치하는 지역을 찾으면
                if (!regionData[j].visited) {  // 방문하지 않은 지역만 추가
                    // 중복 체크
                    bool isDuplicate = false; // 중복 여부 플래그
                    for (int k = 0; k < availableCount; k++) { // 이미 추가된 지역과 비교
                        if (availableRegions[k] == j) {
                            isDuplicate = true; // 중복이면 플래그 설정
                            break; // 내부 반복문 종료
                        }
                    }
                    if (!isDuplicate) {
                        availableRegions[availableCount++] = j; // 중복이 아니면 인덱스 추가 및 카운트 증가
                    }
                }
                break; // 내부 반복문 종료
            }
        }
    }
    
    if (availableCount == 0) { // 방문하지 않은 인접 지역이 없는 경우
        int unvisitedRegions[MAX_REGIONS]; // 방문하지 않은 지역 인덱스 배열
        int unvisitedCount = 0; // 방문하지 않은 지역 수
        
        for (int i = 0; i < MAX_REGIONS; i++) { // 모든 지역을 순회하며
            if (!regionData[i].visited) { // 방문하지 않은 지역이면
                unvisitedRegions[unvisitedCount++] = i; // 인덱스 추가 및 카운트 증가
            }
        }
        
        if (unvisitedCount > 0) { // 방문하지 않은 지역이 하나라도 있으면
            int cost = gPlayer.money / 2; // 이동 비용(보유전의 50%) 계산
            gPlayer.money -= cost; // 이동 비용 차감
            startDialogue(2000); // 이벤트 대사 출력
            printText("배를 타고 이동할 지역을 선택하세요:\n"); // 안내 메시지 출력
            char buffer[128]; // 메시지 버퍼
            sprintf(buffer, "이동 비용으로 %d전을 지불했습니다. (남은 전: %d전)\n", cost, gPlayer.money); // 비용 안내 메시지 생성
            printText(buffer); // 메시지 출력
            
            for (int i = 0; i < unvisitedCount; i++) { // 방문하지 않은 지역 목록 출력
                char buffer[64]; // 지역명 출력용 버퍼
                sprintf(buffer, "%d. %s\n", i+1, regionData[unvisitedRegions[i]].name); // 번호와 지역명 출력
                printText(buffer); // 메시지 출력
            }
            
            printText("숫자를 입력하세요: "); // 입력 안내 메시지 출력
            int choice = getIntInput() - 1; // 사용자 입력(1부터 시작) 받아서 0부터 시작하는 인덱스로 변환
            while (choice < 0 || choice >= unvisitedCount) { // 잘못된 입력일 경우 반복
                printText("\n잘못된 선택입니다. 다시 선택하세요.\n"); // 안내 메시지 출력
                printText("숫자를 입력하세요: "); // 입력 안내 메시지 출력
                choice = getIntInput() - 1; // 다시 입력 받기
            }
            
            int nextIndex = unvisitedRegions[choice]; // 선택한 지역 인덱스
            strcpy(currentRegion, regionData[nextIndex].name); // 현재 지역명 갱신
            regionData[nextIndex].visited = 1; // 방문 표시
            return 1; // 성공 반환
        }
        return 0; // 방문하지 않은 지역이 없으면 실패 반환
    }
    
    printText("\n지도 아이템을 사용하여 다음 지역을 선택하세요:\n"); // 지도 아이템 사용 안내 메시지 출력
    for (int i = 0; i < availableCount; i++) { // 방문 가능한 지역 목록 출력
        char buffer[64]; // 지역명 출력용 버퍼
        sprintf(buffer, "%d. %s\n", i+1, regionData[availableRegions[i]].name); // 번호와 지역명 출력
        printText(buffer); // 메시지 출력
    }
    printText("숫자를 입력하세요: "); // 입력 안내 메시지 출력
    int choice = getIntInput() - 1; // 사용자 입력(1부터 시작) 받아서 0부터 시작하는 인덱스로 변환
    while (choice < 0 || choice >= availableCount) { // 잘못된 입력일 경우 반복
        printText("\n잘못된 선택입니다. 다시 선택하세요.\n"); // 안내 메시지 출력
        printText("숫자를 입력하세요: "); // 입력 안내 메시지 출력
        choice = getIntInput() - 1; // 다시 입력 받기
    }
    int nextIndex = availableRegions[choice]; // 선택한 지역 인덱스
    strcpy(currentRegion, regionData[nextIndex].name); // 현재 지역명 갱신
    regionData[nextIndex].visited = 1; // 방문 표시
    return 1; // 성공 반환
}

void displayConnectedRegions(void) {
    LOG_FUNCTION_EXECUTION("displayConnectedRegions"); // 함수 실행 시 로그 기록
    int currentIndex = -1; // 현재 지역 인덱스(초기값 -1)
    for (int i = 0; i < MAX_REGIONS; i++) { // 모든 지역을 순회하며
        if (strcmp(regionData[i].name, currentRegion) == 0) { // 현재 지역명과 일치하는 지역을 찾으면
            currentIndex = i; // 인덱스 저장
            break; // 반복문 종료
        }
    }
    
    if (currentIndex == -1) return; // 현재 지역을 찾지 못하면 함수 종료
    
    // 연결된 지역 표시 (디버그 모드에서만)
    if (gameSettings.debugMode) { // 디버그 모드가 활성화된 경우
        printText("\n연결된 지역:\n"); // 안내 메시지 출력
        for (int i = 0; i < regionData[currentIndex].connectedCount; i++) { // 연결된 지역 수만큼 반복
            printText(regionData[currentIndex].connected[i]); // 연결된 지역명 출력
            if (i < regionData[currentIndex].connectedCount - 1) { // 마지막이 아니면
                printText(", "); // 쉼표 출력
            }
        }
        fastSleep(500); // 0.5초 대기
        printText("\n"); // 줄바꿈 출력
    }
}

// 지역 데이터 저장 함수, 파일 포인터를 받아 현재 지역명과 방문 기록을 파일에 저장
void saveRegionData(FILE* file) {
    LOG_FUNCTION_EXECUTION("saveRegionData"); // 함수 실행 시 로그 기록
    if (!file) return; // 파일 포인터가 NULL이면 함수 종료
    
    // 현재 지역명 저장
    fwrite(currentRegion, sizeof(char), REGION_NAME_LENGTH, file); // 현재 지역명 문자열을 파일에 저장
    
    // 각 지역의 방문 기록 저장
    for (int i = 0; i < MAX_REGIONS; i++) { // 모든 지역을 순회하며
        fwrite(&regionData[i].visited, sizeof(int), 1, file); // 방문 여부를 파일에 저장
    }
}

// 지역 데이터 불러오기 함수, 파일 포인터를 받아 현재 지역명과 방문 기록을 파일에서 읽어옴
void loadRegionData(FILE* file) {
    LOG_FUNCTION_EXECUTION("loadRegionData"); // 함수 실행 시 로그 기록
    if (!file) return; // 파일 포인터가 NULL이면 함수 종료
    
    // 현재 지역명 불러오기
    fread(currentRegion, sizeof(char), REGION_NAME_LENGTH, file); // 파일에서 현재 지역명 문자열을 읽어옴
    
    // 각 지역의 방문 기록 불러오기
    for (int i = 0; i < MAX_REGIONS; i++) { // 모든 지역을 순회하며
        fread(&regionData[i].visited, sizeof(int), 1, file); // 방문 여부를 파일에서 읽어옴
    }
}

// 모든 지역 방문 여부를 확인하는 함수, 모든 지역의 visited 값이 1이면 1 반환, 아니면 0 반환
int isAllRegionsVisited(void) {
    LOG_FUNCTION_EXECUTION("isAllRegionsVisited"); // 함수 실행 시 로그 기록
    for (int i = 0; i < MAX_REGIONS; i++) { // 모든 지역을 순회하며
        if (!regionData[i].visited) return 0; // 방문하지 않은 지역이 있으면 0 반환
    }
    return 1; // 모든 지역을 방문했으면 1 반환
}

// 방문하지 않은 지역 목록을 regions 배열에 최대 maxCount개까지 복사하고, 실제 복사한 개수를 반환하는 함수
int getUnvisitedRegions(char regions[][REGION_NAME_LENGTH], int maxCount) {
    LOG_FUNCTION_EXECUTION("getUnvisitedRegions"); // 함수 실행 시 로그 기록
    int count = 0; // 복사한 지역 수
    
    for (int i = 0; i < MAX_REGIONS && count < maxCount; i++) { // 모든 지역을 순회하며, 최대 maxCount개까지만
        if (!regionData[i].visited) { // 방문하지 않은 지역이면
            strcpy(regions[count], regionData[i].name); // 지역명 복사
            count++; // 복사한 지역 수 증가
        }
    }
    
    return count; // 복사한 지역 수 반환
} 