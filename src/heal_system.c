#include <stdio.h>                    // 표준 입출력 함수를 사용하기 위한 헤더 파일
#include <windows.h>                  // Windows API 함수를 사용하기 위한 헤더 파일
#include "heal_system.h"              // 회복 시스템 관련 함수 선언이 포함된 헤더 파일
#include "text.h"                     // 텍스트 출력 관련 함수들이 포함된 헤더 파일
#include "input.h"                    // 사용자 입력 처리 함수들이 포함된 헤더 파일
#include "party.h"                    // 파티 관리 관련 함수들이 포함된 헤더 파일
#include "hp_system.h"                // HP 계산 관련 함수들이 포함된 헤더 파일
#include "../core/state.h"            // 게임 상태 관리 관련 헤더 파일
#include "reward.h"                   // 보상 시스템 관련 함수들이 포함된 헤더 파일
#include "logger.h"                   // 로깅 시스템 관련 함수들이 포함된 헤더 파일

// 현재 사용 중인 아이템을 가리키는 전역 포인터 변수 (회복 시스템에서 사용할 아이템)
const Item* currentItem = NULL;

// 회복 시스템 초기화 함수 (시스템 시작 시 호출됨)
void initHealSystem() {
    LOG_FUNCTION_EXECUTION("initHealSystem");    // 함수 실행을 로그에 기록
    // 향후 구현 예정 (현재는 로깅만 수행)
}

// 회복 시스템 정리 함수 (시스템 종료 시 호출됨)
void cleanupHealSystem() {
    LOG_FUNCTION_EXECUTION("cleanupHealSystem"); // 함수 실행을 로그에 기록
    // 향후 구현 예정 (현재는 로깅만 수행)
}

// 요괴 HP 회복 함수 (아이템을 사용하여 요괴의 체력을 회복시킴)
int healYokai(Yokai* targetYokai) {
    LOG_FUNCTION_EXECUTION("healYokai");         // 함수 실행을 로그에 기록
    if (!currentItem) return -1;                 // 현재 아이템이 없으면 -1 반환 (실패)
    
    switch (currentItem->type) {                 // 아이템 타입에 따라 다른 처리 수행
        case ITEM_HEAL:                          // 회복 아이템인 경우
            if (!targetYokai) return -1;         // 대상 요괴가 없으면 -1 반환
            
            // 탕국과 막걸리는 기절한 요괴만 회복 가능한 특수 아이템
            if (strcmp(currentItem->name, "탕국") == 0 || strcmp(currentItem->name, "막걸리") == 0) {
                if (targetYokai->currentHP > 0) {    // HP가 0보다 크면 (기절하지 않았으면)
                    printTextAndWait("\n이 아이템은 기절한 요괴만 회복할 수 있습니다!"); // 오류 메시지 출력
                    return -1;                       // -1 반환 (실패)
                }
            }
            // 그 외 회복 아이템은 기절한 요괴를 회복할 수 없음
            else if (targetYokai->currentHP <= 0) {  // HP가 0 이하면 (기절한 상태)
                printTextAndWait("\n기절한 요괴는 회복할 수 없습니다!"); // 오류 메시지 출력
                fastSleep(500);                      // 0.5초 대기
                return -1;                           // -1 반환 (실패)
            }

            // HP가 이미 최대인 경우 회복 불필요
            float maxHP = calculateHP(targetYokai);  // 요괴의 최대 HP 계산
            if (targetYokai->currentHP >= maxHP) {   // 현재 HP가 최대 HP 이상이면
                printTextAndWait("\n이미 체력이 최대입니다!"); // 오류 메시지 출력
                return -1;                           // -1 반환 (실패)
            }
            
            if (strcmp(currentItem->name, "제삿밥") == 0) {  // 제삿밥 아이템인 경우
                float healAmount = maxHP * 0.3f;             // 최대 HP의 30%만큼 회복량 계산
                float oldHP = targetYokai->currentHP;        // 회복 전 HP 저장
                targetYokai->currentHP += healAmount;        // HP에 회복량 추가
                if (targetYokai->currentHP > maxHP) {        // 최대 HP를 초과하면
                    targetYokai->currentHP = maxHP;          // 최대 HP로 제한
                }
                float actualHeal = targetYokai->currentHP - oldHP; // 실제 회복량 계산
                char buffer[256];                             // 메시지 출력용 버퍼
                sprintf(buffer, "\n%s의 체력이 %.0f 회복되었습니다!\n", targetYokai->name, actualHeal); // 회복 메시지 생성
                printText(buffer);                           // 메시지 출력
                fastSleep(500);                              // 0.5초 대기
                return 1;                                    // 1 반환 (성공)
            }
            else if (strcmp(currentItem->name, "나물") == 0) {  // 나물 아이템인 경우
                float healAmount = maxHP * 0.5f;                 // 최대 HP의 50%만큼 회복량 계산
                float oldHP = targetYokai->currentHP;            // 회복 전 HP 저장
                targetYokai->currentHP += healAmount;            // HP에 회복량 추가
                if (targetYokai->currentHP > maxHP) {            // 최대 HP를 초과하면
                    targetYokai->currentHP = maxHP;              // 최대 HP로 제한
                }
                float actualHeal = targetYokai->currentHP - oldHP; // 실제 회복량 계산
                char buffer[256];                                 // 메시지 출력용 버퍼
                sprintf(buffer, "\n%s의 체력이 %.0f 회복되었습니다!\n", targetYokai->name, actualHeal); // 회복 메시지 생성
                printText(buffer);                               // 메시지 출력
                fastSleep(500);                                  // 0.5초 대기
                return 1;                                        // 1 반환 (성공)
            }
            else if (strcmp(currentItem->name, "탕국") == 0) {  // 탕국 아이템인 경우
                // 탕국은 기절한 요괴만 회복 가능 (이미 위에서 체크했지만 재확인)
                if (targetYokai->currentHP > 0) {                // HP가 0보다 크면 (기절하지 않았으면)
                    printTextAndWait("\n탕국은 기절한 요괴만 회복할 수 있습니다!"); // 오류 메시지 출력
                    return -1;                                   // -1 반환 (실패)
                }
                float healAmount = maxHP * 0.7f;                 // 최대 HP의 70%만큼 회복량 계산
                float oldHP = targetYokai->currentHP;            // 회복 전 HP 저장
                targetYokai->currentHP += healAmount;            // HP에 회복량 추가
                if (targetYokai->currentHP > maxHP) {            // 최대 HP를 초과하면
                    targetYokai->currentHP = maxHP;              // 최대 HP로 제한
                }
                float actualHeal = targetYokai->currentHP - oldHP; // 실제 회복량 계산
                char buffer[256];                                 // 메시지 출력용 버퍼
                sprintf(buffer, "\n%s의 체력이 %.0f 회복되었습니다!\n", targetYokai->name, actualHeal); // 회복 메시지 생성
                printText(buffer);                               // 메시지 출력
                fastSleep(500);                                  // 0.5초 대기
                return 1;                                        // 1 반환 (성공)
            }
            else if (strcmp(currentItem->name, "막걸리") == 0) { // 막걸리 아이템인 경우
                // 막걸리는 기절한 요괴만 회복 가능 (이미 위에서 체크했지만 재확인)
                if (targetYokai->currentHP > 0) {                // HP가 0보다 크면 (기절하지 않았으면)
                    printTextAndWait("\n막걸리는 기절한 요괴만 회복할 수 있습니다!"); // 오류 메시지 출력
                    return -1;                                   // -1 반환 (실패)
                }
                float healAmount = maxHP * 1.0f;                 // 최대 HP의 100%만큼 회복량 계산 (완전 회복)
                float oldHP = targetYokai->currentHP;            // 회복 전 HP 저장
                targetYokai->currentHP += healAmount;            // HP에 회복량 추가
                if (targetYokai->currentHP > maxHP) {            // 최대 HP를 초과하면
                    targetYokai->currentHP = maxHP;              // 최대 HP로 제한
                }
                float actualHeal = targetYokai->currentHP - oldHP; // 실제 회복량 계산
                char buffer[256];                                 // 메시지 출력용 버퍼
                sprintf(buffer, "\n%s의 체력이 %.0f 회복되었습니다!\n", targetYokai->name, actualHeal); // 회복 메시지 생성
                printText(buffer);                               // 메시지 출력
                fastSleep(500);                                  // 0.5초 대기
                return 1;                                        // 1 반환 (성공)
            }
            break;                                               // ITEM_HEAL 케이스 종료
        case ITEM_TALISMAN:                                      // 부적 아이템인 경우
            if (!targetYokai) return -1;                         // 대상 요괴가 없으면 -1 반환
            if (useTalisman(currentItem, targetYokai)) {         // 부적 사용 함수 호출
                char buffer[256];                                // 메시지 출력용 버퍼
                sprintf(buffer, "\n%s가 %s를 사용했습니다!\n", targetYokai->name, currentItem->name); // 사용 메시지 생성
                printText(buffer);                               // 메시지 출력
                fastSleep(500);                                  // 0.5초 대기
                return 1;                                        // 1 반환 (성공)
            }
            break;                                               // ITEM_TALISMAN 케이스 종료
        case ITEM_YANGGAENG:                                     // 양갱 아이템인 경우
            if (strcmp(currentItem->name, "이상한 양갱") == 0) {  // 이상한 양갱인 경우
                printTextAndWait("\n이상한 양갱을 사용합니다...\n"); // 사용 시작 메시지 출력
                
                // 모든 동료 요괴의 레벨을 1 증가시키는 특수 효과
                for (int i = 0; i < gPartyCount; i++) {          // 파티의 모든 요괴에 대해 반복
                    float oldMaxHP = calculateHP(&gParty[i]);    // 레벨업 전 최대 HP 저장
                    int oldLevel = gParty[i].level;              // 레벨업 전 레벨 저장
                    gParty[i].level++;                           // 레벨 1 증가
                    float newMaxHP = calculateHP(&gParty[i]);    // 레벨업 후 최대 HP 계산
                    float hpIncrease = newMaxHP - oldMaxHP;      // HP 증가량 계산
                    
                    // 기절 상태가 아닐 때만 현재 HP 증가 (기절한 요괴는 HP 증가 안됨)
                    if (gParty[i].status != YOKAI_FAINTED) {     // 기절 상태가 아니면
                        gParty[i].currentHP += hpIncrease;       // HP 증가량만큼 현재 HP 증가
                    }
                    
                    // 레벨업 메시지 출력
                    char buffer[256];                            // 메시지 출력용 버퍼
                    sprintf(buffer, "\n%s의 레벨이 %d에서 %d로 상승했습니다!\n", 
                        gParty[i].name, oldLevel, gParty[i].level); // 레벨업 메시지 생성
                    printTextAndWait(buffer);                    // 메시지 출력 및 대기
                }
                printTextAndWait("\n모든 동료 요괴의 레벨이 상승했습니다!\n"); // 전체 완료 메시지 출력
                fastSleep(500);                                  // 0.5초 대기
                return 1;                                        // 1 반환 (성공)
            } else {                                             // 일반 양갱인 경우
                if (!targetYokai) return -1;                     // 대상 요괴가 없으면 -1 반환
                if (useYanggaeng(currentItem, targetYokai)) {    // 양갱 사용 함수 호출
                    char buffer[256];                            // 메시지 출력용 버퍼
                    sprintf(buffer, "\n%s가 %s를 먹었습니다!\n", targetYokai->name, currentItem->name); // 사용 메시지 생성
                    printText(buffer);                           // 메시지 출력
                    fastSleep(500);                              // 0.5초 대기
                    return 1;                                    // 1 반환 (성공)
                }
            }
            break;                                               // ITEM_YANGGAENG 케이스 종료
        default:                                                 // 기타 아이템 타입
            break;                                               // 기본 케이스 종료
    }
    return -1;                                                   // 모든 케이스에서 성공하지 못하면 -1 반환 (실패)
}

// 회복할 요괴 선택 함수 (사용자가 회복할 요괴를 선택할 수 있게 함)
Yokai* selectYokaiToHeal() {
    LOG_FUNCTION_EXECUTION("selectYokaiToHeal");                // 함수 실행을 로그에 기록
    // 이상한 양갱인 경우 바로 NULL 반환 (모든 요괴에 적용되므로 개별 선택 불필요)
    if (currentItem && strcmp(currentItem->name, "이상한 양갱") == 0) {
        return NULL;                                             // NULL 반환 (개별 선택 불필요)
    }

    // 평범한 양갱이나 고급 양갱인 경우 특별한 메시지 출력
    if (currentItem && (strcmp(currentItem->name, "평범한 양갱") == 0 || strcmp(currentItem->name, "고급 양갱") == 0)) {
        printText("\n양갱을 먹을 요괴를 선택하세요:\n");           // 양갱 선택 메시지 출력
    } else if (currentItem && strcmp(currentItem->name, "버섯죽") == 0) {
        printText("\n잊은기술을 배우게할 요괴를 선택하세요:\n");   // 버섯죽 선택 메시지 출력
    } else {
        printText("\n회복할 요괴를 선택하세요:\n");               // 일반 회복 선택 메시지 출력
    }

    for (int i = 0; i < gPartyCount; i++) {                     // 파티의 모든 요괴에 대해 반복
        char buffer[256];                                        // 메시지 출력용 버퍼
        float maxHP = calculateHP(&gParty[i]);                  // 요괴의 최대 HP 계산
        
        // 기절 상태 표시를 위한 변수 초기화
        const char* statusText = "";                             // 상태 텍스트 초기화
        const char* statusColor = "";                            // 상태 색상 코드 초기화
        if (gParty[i].status == YOKAI_FAINTED) {                // 요괴가 기절 상태인 경우
            statusText = " [기절]";                              // 기절 상태 텍스트 설정
            statusColor = "\033[31m";                            // 빨간색 색상 코드 설정
        }
        
        sprintf(buffer, "%d. %s Lv.%d (HP: %.0f/%.0f)%s%s\033[0m\n", 
            i+1,                                                 // 선택 번호
            gParty[i].name,                                      // 요괴 이름
            gParty[i].level,                                     // 요괴 레벨
            gParty[i].currentHP,                                 // 현재 HP
            maxHP,                                               // 최대 HP
            statusColor,                                         // 상태 색상 코드
            statusText);                                         // 상태 텍스트
        printTextAndWait(buffer);                                // 요괴 정보 출력 및 대기
    }
    printText("0. 뒤로 가기\n");                                 // 뒤로 가기 옵션 출력
    printText("숫자를 입력해주세요: ");                          // 입력 요청 메시지 출력
    
    int choice = getIntInput();                                  // 사용자 입력 받기
    if (choice == 0) {                                           // 0을 선택한 경우 (뒤로 가기)
        return NULL;                                             // NULL 반환
    }
    
    if (choice > 0 && choice <= gPartyCount) {                  // 유효한 선택 범위인 경우
        return &gParty[choice - 1];                              // 선택된 요괴의 포인터 반환
    }
    
    printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요.");    // 잘못된 선택 시 오류 메시지 출력
    return NULL;                                                 // NULL 반환 (재선택 필요)
}

// 기술 선택 함수 (PP 회복할 기술을 선택할 수 있게 함)
int selectMoveToHeal(Yokai* targetYokai) {
    LOG_FUNCTION_EXECUTION("selectMoveToHeal");                  // 함수 실행을 로그에 기록
    printText("\n=== 회복할 기술 선택 ===\n");                   // 제목 출력
    
    // 기술 목록 표시 (각 기술의 타입에 따라 색상 다르게 표시)
    for (int i = 0; i < targetYokai->moveCount; i++) {          // 요괴가 가진 모든 기술에 대해 반복
        char buffer[256];                                        // 메시지 출력용 버퍼
        const char* colorCode;                                   // 색상 코드 변수
        switch (targetYokai->moves[i].move.type) {              // 기술 타입에 따라 색상 결정
            case TYPE_EVIL_SPIRIT:                               // 악령 타입
                colorCode = "\033[31m";                          // 빨간색
                break;
            case TYPE_GHOST:                                     // 귀신 타입
                colorCode = "\033[35m";                          // 보라색
                break;
            case TYPE_MONSTER:                                   // 괴물 타입
                colorCode = "\033[33m";                          // 노란색
                break;
            case TYPE_HUMAN:                                     // 인간 타입
                colorCode = "\033[36m";                          // 청록색
                break;
            case TYPE_ANIMAL:                                    // 동물 타입
                colorCode = "\033[32m";                          // 초록색
                break;
            default:                                             // 기타 타입
                colorCode = "\033[0m";                           // 기본색
        }
        
        sprintf(buffer, "%d. %s%s%s\033[0m (PP: %d/%d)\n",       // 기술 정보 메시지 생성
            i + 1,                                               // 선택 번호
            colorCode,                                           // 색상 코드
            targetYokai->moves[i].move.name,                     // 기술 이름
            colorCode,                                           // 색상 코드 (닫기)
            targetYokai->moves[i].currentPP,                     // 현재 PP
            targetYokai->moves[i].move.pp);                      // 최대 PP
        printText(buffer);                                       // 기술 정보 출력
    }
    printText("0. 뒤로 가기\n");                                 // 뒤로 가기 옵션 출력
    printText("숫자를 입력하세요: ");                            // 입력 요청 메시지 출력
    
    // 사용자 입력 받기
    int choice = getIntInput();                                  // 정수 입력 받기
    if (choice == 0) {                                           // 0을 선택한 경우 (뒤로 가기)
        return -1;                                               // -1 반환 (뒤로 가기)
    }
    if (choice < 1 || choice > targetYokai->moveCount) {        // 유효하지 않은 선택 범위인 경우
        printTextAndWait("\n잘못된 선택입니다. 다시 선택하세요."); // 오류 메시지 출력
        return selectMoveToHeal(targetYokai);                    // 재귀 호출로 다시 선택
    }
    return choice - 1;                                           // 선택된 기술의 인덱스 반환 (0부터 시작)
}

// 단일 기술 PP 회복 처리 함수 (특정 기술의 PP를 회복시킴)
void healSingleMovePP(Yokai* targetYokai, int moveIndex, int ppAmount) {
    LOG_FUNCTION_EXECUTION("healSingleMovePP");                  // 함수 실행을 로그에 기록
    if (targetYokai == NULL || moveIndex < 0 || moveIndex >= targetYokai->moveCount) { // 유효성 검사
        printTextAndWait("\n잘못된 기술 선택입니다.");            // 오류 메시지 출력
        return;                                                  // 함수 종료
    }
    
    MoveInstance* move = &targetYokai->moves[moveIndex];         // 선택된 기술의 인스턴스 가져오기
    int oldPP = move->currentPP;                                 // 회복 전 PP 저장
    
    if (ppAmount == -1) {                                        // -1인 경우 (최대 PP로 회복)
        move->currentPP = move->move.pp;                         // 최대 PP로 설정
    } else {                                                     // 특정 양만큼 회복
        move->currentPP += ppAmount;                             // PP에 회복량 추가
        if (move->currentPP > move->move.pp) {                   // 최대 PP를 초과하면
            move->currentPP = move->move.pp;                     // 최대 PP로 제한
        }
    }
    
    char buffer[256];                                            // 메시지 출력용 버퍼
    sprintf(buffer, "\n%s의 %s PP가 %d에서 %d로 회복되었습니다!", // 회복 완료 메시지 생성
        targetYokai->name,                                       // 요괴 이름
        move->move.name,                                         // 기술 이름
        oldPP,                                                   // 회복 전 PP
        move->currentPP);                                        // 회복 후 PP
    printTextAndWait(buffer);                                    // 메시지 출력 및 대기
    fastSleep(500);                                              // 0.5초 대기
}

// 모든 기술 PP 회복 처리 함수 (요괴의 모든 기술 PP를 회복시킴)
void healAllMovesPP(Yokai* targetYokai) {
    LOG_FUNCTION_EXECUTION("healAllMovesPP");                    // 함수 실행을 로그에 기록
    if (targetYokai == NULL) {                                   // 대상 요괴가 없는 경우
        printTextAndWait("\n회복할 요괴가 선택되지 않았습니다.");   // 오류 메시지 출력
        return;                                                  // 함수 종료
    }
    
    for (int i = 0; i < targetYokai->moveCount; i++) {          // 모든 기술에 대해 반복
        targetYokai->moves[i].currentPP = targetYokai->moves[i].move.pp; // 최대 PP로 설정
    }
    
    char buffer[256];                                            // 메시지 출력용 버퍼
    sprintf(buffer, "\n%s의 모든 기술 PP가 회복되었습니다!", targetYokai->name); // 완료 메시지 생성
    printTextAndWait(buffer);                                    // 메시지 출력 및 대기
    fastSleep(500);                                              // 0.5초 대기
}

// 기술 PP 회복 처리 함수 (사용자가 기술을 선택하여 PP를 회복시킴)
void healMovePP(Yokai* targetYokai, int ppAmount) {
    LOG_FUNCTION_EXECUTION("healMovePP");                        // 함수 실행을 로그에 기록
    if (targetYokai == NULL) {                                   // 대상 요괴가 없는 경우
        printTextAndWait("\n회복할 요괴가 선택되지 않았습니다.");   // 오류 메시지 출력
        return;                                                  // 함수 종료
    }
    
    int moveIndex = selectMoveToHeal(targetYokai);               // 회복할 기술 선택
    if (moveIndex == -1) {                                       // 기술 선택에서 뒤로가기를 선택한 경우
        // 요괴 선택창으로 복귀
        Yokai* newTarget = selectYokaiToHeal();                  // 새로운 요괴 선택
        if (newTarget != NULL) {                                 // 새로운 요괴가 선택된 경우
            healMovePP(newTarget, ppAmount);                     // 재귀 호출로 새로운 요괴에 대해 PP 회복
        } else {                                                 // 요괴 선택에서도 뒤로가기를 선택한 경우
            // 아이템 보상창으로 복귀
            itemRewardSystem();                                  // 아이템 보상 시스템 호출
        }
        return;                                                  // 함수 종료
    }
    if (moveIndex != -1) {                                       // 유효한 기술이 선택된 경우
        healSingleMovePP(targetYokai, moveIndex, ppAmount);      // 선택된 기술의 PP 회복
    }
} 