#include <stdio.h>  // 표준 입출력 함수 헤더 포함
#include <stdlib.h>  // 표준 라이브러리 함수 헤더 포함
#include <string.h>  // 문자열 처리 함수 헤더 포함
#include "reward.h"  // 보상 시스템 헤더 포함
#include "text.h"  // 텍스트 출력 헤더 포함
#include "input.h"  // 입력 처리 헤더 포함
#include "item.h"  // 아이템 시스템 헤더 포함
#include "game.h"  // 게임 시스템 헤더 포함
#include "party.h"  // 파티 관리 헤더 포함
#include "battle.h"  // 전투 시스템 헤더 포함
#include "../core/state.h"  // 게임 상태 헤더 포함
#include "logger.h"  // 로깅 시스템 헤더 포함
#include <windows.h>  // Windows API 헤더 포함
#include <math.h>  // 수학 함수 헤더 포함

// 필요한 전역 변수는 state 모듈에서 접근

// static 변수들을 전역 변수로 이동
Item candidates[3];  // 보상 후보 아이템 배열
int isInitialized = 0;  // 아이템 후보 초기화 여부
int resetCount = 0;  // 아이템 목록 초기화 횟수
int lastStageNumber = -1;  // 마지막 스테이지 번호

// 전투 보상 전 계산 함수
int calculateBattleReward() {  // 전투 보상 계산 함수 정의
    LOG_FUNCTION_EXECUTION("calculateBattleReward");  // 함수 실행 로그 기록
    // 기본 보상: 100전
    int baseReward = 100;  // 기본 보상 금액 설정
    
    // 스테이지 보너스: 스테이지가 올라갈수록 보상이 점진적으로 증가
    // 예: 1스테이지 = 100, 2스테이지 = 110, 3스테이지 = 121, 4스테이지 = 133, ...
    int stageBonus = (int)(baseReward * pow(1.1, gStage.stageNumber - 1));  // 스테이지 보너스 계산
    
    // 랜덤 보너스: 0-49전
    int randomBonus = rand() % 50;  // 랜덤 보너스 계산
    
    // 레벨 보너스: 적 요괴 레벨당 15전씩 보너스
    int levelBonus = currentEnemy.level * 15;  // 레벨 보너스 계산
    
    // 10의 배수 스테이지에서는 추가 보너스
    if (gStage.stageNumber % 10 == 0) {  // 10의 배수 스테이지인 경우
        // 10의 배수 스테이지에서는 기본 보상의 50% 추가
        stageBonus = (int)(stageBonus * 1.5);  // 스테이지 보너스 50% 증가
    }
    
    // 최종 보상 계산
    return stageBonus + randomBonus + levelBonus;  // 최종 보상 반환
}

// 초기화 비용 계산 함수
static int calculateResetCost(int stageNumber) {  // 초기화 비용 계산 함수 정의
    LOG_FUNCTION_EXECUTION("calculateResetCost");  // 함수 실행 로그 기록
    int baseCost = 100;  // 기본 비용 설정
    int multiplier = 1;  // 기본 배수 설정
    
    // 10스테이지마다 2배씩 증가
    if (stageNumber > 10) {  // 10스테이지 초과인 경우
        multiplier = 1 << ((stageNumber - 1) / 10);  // 2의 (스테이지/10)제곱
    }
    
    return baseCost * multiplier;  // 최종 비용 반환
}

// getRandomItems는 item.c의 것을 사용

// 아이템 등급을 한글로 반환하는 함수
const char* getGradeName(ItemGrade grade) {  // 아이템 등급명 반환 함수 정의
    LOG_FUNCTION_EXECUTION("getGradeName");  // 함수 실행 로그 기록
    switch (grade) {  // 아이템 등급에 따른 이름 반환
        case ITEM_COMMON:  // 일반 등급인 경우
            return "일반";  // 일반 반환
        case ITEM_RARE:  // 희귀 등급인 경우
            return "희귀";  // 희귀 반환
        case ITEM_SUPERRARE:  // 초희귀 등급인 경우
            return "초희귀";  // 초희귀 반환
        default:  // 기본값인 경우
            return "알 수 없음";  // 알 수 없음 반환
    }
}

// 파이널 스테이지 여부를 반환하는 함수
int isFinalStage() {  // 파이널 스테이지 확인 함수 정의
    LOG_FUNCTION_EXECUTION("isFinalStage");  // 함수 실행 로그 기록
    return strcmp(gStage.region, "백두산 정상") == 0;  // 백두산 정상 지역인지 확인
}

// 찹살경단 전용 요괴 선택 함수
static void selectYokaiForChapsalgyungdan(int idx) {  // 찹살경단 요괴 선택 함수 정의
    while (1) {  // 무한 루프 시작
        printText("\n찹살경단을 지니게 할 요괴를 선택하세요:\n");  // 선택 안내 메시지 출력
        for (int i = 0; i < gPartyCount; i++) {  // 파티의 모든 요괴에 대해 반복
            char buffer[256];  // 출력용 버퍼 선언
            sprintf(buffer, "%d. %s Lv.%d (HP: %.0f/%.0f)\033[0m\n",  // 요괴 정보 포맷팅
                i + 1,  // 요괴 번호
                gParty[i].name,  // 요괴 이름
                gParty[i].level,  // 요괴 레벨
                gParty[i].currentHP,  // 현재 HP
                calculateHP(&gParty[i]));  // 최대 HP
            printText(buffer);  // 요괴 정보 출력
        }
        printText("0. 뒤로 가기\n");  // 뒤로가기 옵션 출력
        printText("숫자를 입력해주세요: ");  // 입력 안내 메시지 출력
        int yokaiChoice = getIntInput();  // 사용자 선택 받기
        if (yokaiChoice == 0) {  // 뒤로가기 선택한 경우
            itemRewardSystem();  // 아이템 보상 시스템 호출
            return;  // 함수 종료
        }
        if (yokaiChoice < 1 || yokaiChoice > gPartyCount) {  // 유효하지 않은 선택인 경우
            printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");  // 오류 메시지 출력
            continue;  // 루프 계속
        }
        Yokai* targetYokai = &gParty[yokaiChoice - 1];  // 선택된 요괴 포인터 설정
        int hasChapsal = 0;  // 찹살경단 보유 여부 플래그 초기화
        for (int i = 0; i < targetYokai->yokaiInventoryCount; i++) {  // 요괴 인벤토리 순회
            if (strcmp(targetYokai->yokaiInventory[i].item.name, "찹살경단") == 0) {  // 찹살경단 보유 확인
                hasChapsal = 1;  // 찹살경단 보유 플래그 설정
                break;  // 루프 종료
            }
        }
        if (hasChapsal) {  // 이미 찹살경단을 가지고 있는 경우
            printTextAndWait("\n이미 찹살경단을 가지고 있습니다!");  // 오류 메시지 출력
            fastSleep(500);  // 0.5초 대기
            continue;  // 요괴 선택 메뉴 반복
        } else {  // 찹살경단을 가지고 있지 않은 경우
            if (addItemToYokaiInventory(targetYokai, &candidates[idx])) {  // 요괴 인벤토리에 아이템 추가 성공한 경우
                printTextAndWait("\n찹살경단을 요괴 인벤토리에 추가했습니다!");  // 성공 메시지 출력
            } else {  // 아이템 추가 실패한 경우
                printTextAndWait("\n찹살경단은 1개만 보유할 수 있습니다!");  // 실패 메시지 출력
            }
            fastSleep(500);  // 0.5초 대기
            break;  // 루프 종료
        }
    }
}

// 아이템 보상 시스템
void itemRewardSystem() {  // 아이템 보상 시스템 함수 정의
    LOG_FUNCTION_EXECUTION("itemRewardSystem");  // 함수 실행 로그 기록
    // 스테이지가 바뀌면 resetCount와 isInitialized를 초기화
    if (lastStageNumber != gStage.stageNumber) {  // 스테이지 번호가 변경된 경우
        resetCount = 0;  // 초기화 횟수 초기화
        isInitialized = 0;  // 초기화 여부 초기화
        lastStageNumber = gStage.stageNumber;  // 마지막 스테이지 번호 업데이트
    }

    // 현재 스테이지 번호에 따른 초기화 비용 계산
    int resetCost = calculateResetCost(gStage.stageNumber);  // 초기화 비용 계산
    
    // 초기화 횟수에 따른 추가 비용 계산
    for (int i = 0; i < resetCount; i++) {  // 초기화 횟수만큼 반복
        resetCost *= 2;  // 비용을 2배씩 증가
    }
    
    // 처음 호출될 때만 랜덤 아이템 생성
    if (!isInitialized) {  // 초기화되지 않은 경우
        // 중복 제거를 위해 반복
        int valid = 0;  // 유효성 플래그 초기화
        while (!valid) {  // 유효하지 않은 동안 반복
            getRandomItems(candidates, 3);  // 랜덤 아이템 3개 생성
            valid = 1;  // 유효성 플래그 설정
            for (int i = 0; i < 3; i++) {  // 모든 후보 아이템에 대해 반복
                // 보스 스테이지이면서 파이널 스테이지가 아닐 때만 회복형 아이템 제외
                if (gStage.isBossStage && !isFinalStage() && candidates[i].type == ITEM_HEAL) {  // 보스 스테이지에서 회복 아이템인 경우
                    valid = 0;  // 유효성 플래그 해제
                    break;  // 루프 종료
                }
                for (int j = i + 1; j < 3; j++) {  // 다른 후보 아이템들과 비교
                    if (strcmp(candidates[i].name, candidates[j].name) == 0) {  // 중복 아이템인 경우
                        valid = 0;  // 유효성 플래그 해제
                        break;  // 루프 종료
                    }
                }
                // 지도 아이템 중복 방지: 이미 보유 중이면 후보에서 제외
                if (strcmp(candidates[i].name, "지도") == 0) {  // 지도 아이템인 경우
                    for (int k = 0; k < inventoryCount; k++) {  // 인벤토리 순회
                        if (strcmp(inventory[k].item.name, "지도") == 0) {  // 이미 지도를 보유한 경우
                            valid = 0;  // 유효성 플래그 해제
                            break;  // 루프 종료
                        }
                    }
                }
                if (!valid) break;  // 유효하지 않으면 루프 종료
            }
        }
        isInitialized = 1;  // 초기화 완료 플래그 설정
    }
    
    printText("\n=== 아이템 보상 ===\n");  // 제목 출력
    for (int i = 0; i < 3; i++) {  // 3개의 후보 아이템에 대해 반복
        char buffer[256];  // 출력용 버퍼 선언
        const char* colorCode;  // 색상 코드 변수 선언
        switch (candidates[i].grade) {  // 아이템 등급에 따른 색상 설정
            case ITEM_COMMON:  // 일반 등급인 경우
                colorCode = "\033[0m";  // 기본색 (흰색)
                break;  // 케이스 종료
            case ITEM_RARE:  // 희귀 등급인 경우
                colorCode = "\033[33m";  // 노란색
                break;  // 케이스 종료
            case ITEM_SUPERRARE:  // 초희귀 등급인 경우
                colorCode = "\033[31m";  // 빨간색
                break;  // 케이스 종료
            default:  // 기본 등급인 경우
                colorCode = "\033[0m";  // 기본색 설정
        }
        sprintf(buffer, "%d. %s%s [%s]\033[0m - %s\n", i+1,  // 아이템 정보 포맷팅
            colorCode,  // 색상 코드
            candidates[i].name,  // 아이템 이름
            getGradeName(candidates[i].grade),  // 등급명
            candidates[i].desc);  // 아이템 설명
        printText(buffer);  // 아이템 정보 출력
    }
    char resetBuffer[128];  // 초기화 비용 표시용 버퍼 선언
    sprintf(resetBuffer, "\033[94m4. 아이템 목록 초기화 (%d전)\033[0m\n", resetCost);  // 하드코딩된 200전 대신 resetCost 사용
    printText(resetBuffer);  // 초기화 옵션 출력
    printText("\033[95m5. 아이템을 받지 않고 넘어간다\033[0m\n");  // 밝은 보라
    printText("숫자를 입력하세요: ");  // 입력 안내 메시지 출력
    int idx = getIntInput() - 1;  // 사용자 선택 받기 (0부터 시작하는 인덱스로 변환)
    
    if (idx == 4) {  // 아이템을 받지 않고 넘어가기 선택
        printTextAndWait("\n아이템을 받지 않고 넘어갑니다.");  // 안내 메시지 출력
        isInitialized = 0;  // 초기화 플래그 리셋
        resetCount = 0;  // 초기화 횟수 리셋
        return;  // 함수 종료
    }
    
    if (idx == 3) {  // 아이템 목록 초기화 선택
        if (gPlayer.money >= resetCost) {  // 충분한 돈이 있는 경우
            gPlayer.money -= resetCost;  // 돈 차감
            char buffer[128];  // 메시지용 버퍼 선언
            sprintf(buffer, "\n%d전을 사용하여 아이템 목록을 초기화했습니다! (남은 전: %d)\n", resetCost, gPlayer.money);  // 메시지 포맷팅
            printText(buffer);  // 메시지 출력
            
            // 초기화 횟수 증가
            resetCount++;  // 초기화 횟수 증가
            
            // 아이템 목록 초기화
            isInitialized = 0;  // 초기화 플래그 리셋
            itemRewardSystem();  // 아이템 보상 시스템 재호출
            return;  // 함수 종료
        } else {  // 돈이 부족한 경우
            printTextAndWait("\n전이 부족합니다!");  // 오류 메시지 출력
            itemRewardSystem();  // 아이템 보상 시스템 재호출
            return;  // 함수 종료
        }
    }
    
    if (idx < 0 || idx >= 3) {  // 유효하지 않은 선택인 경우
        printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");  // 오류 메시지 출력
        itemRewardSystem();  // 아이템 보상 시스템 재호출
        return;  // 함수 종료
    }
    
    // 찹살경단 분기를 최우선으로 처리
    if (strcmp(candidates[idx].name, "찹살경단") == 0) {  // 찹살경단인 경우
        selectYokaiForChapsalgyungdan(idx);  // 찹살경단 요괴 선택 함수 호출
        return;  // 함수 종료
    }
    // 요괴형 아이템은 요괴 인벤토리로
    if (strcmp(candidates[idx].name, "돋보기") == 0 ||  // 돋보기인 경우
        strcmp(candidates[idx].name, "복숭아") == 0 ||  // 복숭아인 경우
        strcmp(candidates[idx].name, "고대의 서적") == 0 ||  // 고대의 서적인 경우
        candidates[idx].type == ITEM_YANGGAENG) {  // 양갱형 아이템 추가
        
        // 요괴 선택 메뉴
        if (strcmp(candidates[idx].name, "돋보기") == 0) {  // 돋보기인 경우
            printText("\n돋보기를 장착할 요괴를 선택하세요:\n");  // 안내 메시지 출력
        } else if (strcmp(candidates[idx].name, "복숭아") == 0) {  // 복숭아인 경우
            printText("\n복숭아를 지니게 할 요괴를 선택하세요:\n");  // 안내 메시지 출력
        } else if (strcmp(candidates[idx].name, "고대의 서적") == 0) {  // 고대의 서적인 경우
            printText("\n고대의 서적을 지니게 할 요괴를 선택하세요:\n");  // 안내 메시지 출력
        } else if (candidates[idx].type == ITEM_YANGGAENG) {  // 양갱형 아이템인 경우
            if (strcmp(candidates[idx].name, "이상한 양갱") == 0) {  // 이상한 양갱인 경우
                // 이상한 양갱은 바로 사용
                addItemToInventory(&candidates[idx]);  // 인벤토리에 추가
                return;  // 함수 종료
            }
            printText("\n양갱을 먹을 요괴를 선택하세요:\n");  // 안내 메시지 출력
        } else {  // 기타 아이템인 경우
            printText("\n회복할 요괴를 선택하세요:\n");  // 안내 메시지 출력
        }
        for (int i = 0; i < gPartyCount; i++) {  // 파티의 모든 요괴에 대해 반복
            char buffer[256];  // 출력용 버퍼 선언
            sprintf(buffer, "%d. %s Lv.%d (HP: %.0f/%.0f)\033[0m\n",  // 요괴 정보 포맷팅
                i + 1,  // 요괴 번호
                gParty[i].name,  // 요괴 이름
                gParty[i].level,  // 요괴 레벨
                gParty[i].currentHP,  // 현재 HP
                calculateHP(&gParty[i]));  // 최대 HP
            printText(buffer);  // 요괴 정보 출력
        }
        printText("0. 뒤로 가기\n");  // 뒤로가기 옵션 출력
        printText("숫자를 입력해주세요: ");  // 입력 안내 메시지 출력
        
        int yokaiChoice = getIntInput();  // 사용자 선택 받기
        if (yokaiChoice == 0) {  // 뒤로가기 선택한 경우
            itemRewardSystem();  // 아이템 보상 시스템 재호출
            return;  // 함수 종료
        }
        
        if (yokaiChoice < 1 || yokaiChoice > gPartyCount) {  // 유효하지 않은 선택인 경우
            printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");  // 오류 메시지 출력
            itemRewardSystem();  // 아이템 보상 시스템 재호출
            return;  // 함수 종료
        }
        
        // 선택된 요괴에게 아이템 지급
        Yokai* targetYokai = &gParty[yokaiChoice - 1];  // 선택된 요괴 포인터 설정
        int result = useYokaiItem(&candidates[idx], targetYokai);  // 요괴 아이템 사용
        if (result == 1) {  // 사용 성공한 경우
            // 고급양갱은 메시지 출력하지 않음
            if (strcmp(candidates[idx].name, "고급양갱") != 0) {  // 고급양갱이 아닌 경우
                fastSleep(500);  // 0.5초 대기
            }
        } else if (result == -1) {  // 최대 개수 초과한 경우
            // 최대 개수 초과 시 요괴 선택 메뉴로 복귀
            itemRewardSystem();  // 아이템 보상 시스템 재호출
            return;  // 함수 종료
        } else if (candidates[idx].type == ITEM_YANGGAENG) {  // 양갱형 아이템인 경우
            if (addItemToYokaiInventory(targetYokai, &candidates[idx])) {  // 요괴 인벤토리에 추가 성공한 경우
                // 고급양갱은 메시지 출력하지 않음
                if (strcmp(candidates[idx].name, "고급양갱") != 0) {  // 고급양갱이 아닌 경우
                }
            } else {  // 추가 실패한 경우
                printTextAndWait("\n이미 최대치까지 보유하고 있습니다!");  // 오류 메시지 출력
            }
            fastSleep(500);  // 0.5초 대기
        }
    } else {  // 일반 아이템인 경우
        // 일반 아이템은 플레이어 인벤토리에 추가
        addItemToInventory(&candidates[idx]);  // 인벤토리에 아이템 추가
        
        // 작두나 무당방울의 경우 개수에 따라 메시지 출력 여부 결정
        if (strcmp(candidates[idx].name, "작두") == 0 || strcmp(candidates[idx].name, "무당방울") == 0) {  // 작두나 무당방울인 경우
            int count = 0;  // 개수 변수 초기화
            for (int i = 0; i < inventoryCount; i++) {  // 인벤토리 순회
                if (strcmp(inventory[i].item.name, candidates[idx].name) == 0) {  // 해당 아이템 찾은 경우
                    count = inventory[i].count;  // 개수 저장
                    break;  // 루프 종료
                }
            }
            // 5개 미만일 때만 메시지 출력 (중복 방지 위해 메시지 출력 코드 삭제)
            if (count < 5) {  // 5개 미만인 경우
                fastSleep(500);  // 0.5초 대기
            }
        }
        // 회복형, 양갱형 아이템이 아닌 경우에만 획득 메시지 출력 (중복 방지 위해 메시지 출력 코드 삭제)
        else if (candidates[idx].type != ITEM_HEAL && candidates[idx].type != ITEM_YANGGAENG) {  // 회복형이나 양갱형이 아닌 경우
            fastSleep(500);  // 0.5초 대기
        }
    }
    
    // 보상 선택이 완료되면 초기화 플래그를 리셋
    isInitialized = 0;  // 초기화 플래그 리셋
    resetCount = 0;  // 초기화 횟수도 리셋
}

// 새 게임 시작 시 호출할 초기화 함수
void resetItemRewardSystem(void) {  // 아이템 보상 시스템 초기화 함수 정의
    LOG_FUNCTION_EXECUTION("resetItemRewardSystem");  // 함수 실행 로그 기록
    resetCount = 0;  // 초기화 횟수 리셋
    isInitialized = 0;  // 초기화 플래그 리셋
    lastStageNumber = -1;  // 마지막 스테이지 번호 리셋
} 