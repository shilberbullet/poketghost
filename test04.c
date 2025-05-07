#include <stdio.h> // 입출력을 위한 헤더파일

// 문자열 비교 함수 (strcmp 대신 직접 구현한 함수)
int my_strcmp(const char *a, const char *b) {
    while (*a && *b) {          // 두 문자열의 끝이 아닐 때까지 반복
        if (*a != *b) return 0; // 하나라도 다르면 0 반환 (다르다)
        a++;                    // 다음 문자로 이동
        b++;
    }
    return *a == *b;            // 둘 다 '\0'이면 1 반환 (같다), 아니면 0
}

// 함수 선언
void arithmetic(); // 산술 연산 함수
void bitwise();    // 비트 연산 함수

int main() {
    int choice; // 메뉴 선택 저장 변수

    while (1) { // 무한 반복, 종료 선택 시 break로 탈출
        // 메인 메뉴 출력
        printf("=== 계산기 프로그램 ===\n");
        printf("1. 산술 연산\n");
        printf("2. 비트 연산\n");
        printf("3. 종료\n");
        printf("선택: ");

        if (scanf("%d", &choice) != 1) { // 숫자 아닌 값 입력 시 예외 처리
            printf("잘못된 입력입니다! 다시 시도하세요.\n");
            while (getchar() != '\n'); // 입력 버퍼 비우기
            continue;
        }

        if (choice == 1) {
            arithmetic(); // 산술 연산 수행
        } else if (choice == 2) {
            bitwise(); // 비트 연산 수행
        } else if (choice == 3) {
            printf("프로그램 종료!\n");
            break; // while 문 탈출
        } else {
            printf("잘못된 입력입니다! 다시 시도하세요.\n");
        }
    }

    return 0;
}

// 산술 연산 함수 정의
void arithmetic() {
    char op; // 연산자 저장 변수
    int a, b; // 두 피연산자
    int valid = 0; // 올바른 연산자인지 확인용

    // 올바른 연산자 입력 받을 때까지 반복
    while (!valid) {
        printf("[+] [-] [*] [/] [%%] 선택: ");
        scanf(" %c", &op); // 공백을 포함한 문자 입력

        // 연산자 확인
        if (op == '+' || op == '-' || op == '*' || op == '/' || op == '%') {
            valid = 1;
        } else {
            printf("잘못된 입력입니다! 다시 시도하세요.\n");
        }
    }

    // 두 숫자 입력받기
    while (1) {
        printf("숫자 두 개 입력: ");
        if (scanf("%d %d", &a, &b) != 2) {
            printf("잘못된 입력입니다! 다시 시도하세요.\n");
            while (getchar() != '\n');
            continue;
        }

        // 0으로 나눌 수 없으므로 예외 처리
        if ((op == '/' || op == '%') && b == 0) {
            printf("0으로 나눌 수 없습니다! 다시 시도하세요.\n");
            continue;
        }

        break; // 올바른 입력이므로 반복 종료
    }

    // 결과 출력
    printf("결과: ");
    switch (op) {
        case '+': printf("%d\n", a + b); break;
        case '-': printf("%d\n", a - b); break;
        case '*': printf("%d\n", a * b); break;
        case '/': printf("%d\n", a / b); break;
        case '%': printf("%d\n", a % b); break;
    }
}

// 비트 연산 함수 정의
void bitwise() {
    char op[3]; // 문자열로 연산자 저장 (<<, >> 포함)
    int a, b;
    int valid = 0;

    // 올바른 비트 연산자 입력 받을 때까지 반복
    while (!valid) {
        printf("[&] [|] [^] [<<] [>>] 선택: ");
        scanf("%s", op); // 문자열 입력

        // 우리가 만든 my_strcmp 함수로 비교
        if (my_strcmp(op, "&") || my_strcmp(op, "|") || my_strcmp(op, "^") ||
            my_strcmp(op, "<<") || my_strcmp(op, ">>")) {
            valid = 1;
        } else {
            printf("잘못된 입력입니다! 다시 시도하세요.\n");
        }
    }

    // 숫자 두 개 입력
    while (1) {
        printf("숫자 두 개 입력: ");
        if (scanf("%d %d", &a, &b) != 2) {
            printf("잘못된 입력입니다! 다시 시도하세요.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    }

    // 연산 수행 및 결과 출력
    printf("결과: ");
    if (my_strcmp(op, "&")) {
        printf("%d\n", a & b);
    } else if (my_strcmp(op, "|")) {
        printf("%d\n", a | b);
    } else if (my_strcmp(op, "^")) {
        printf("%d\n", a ^ b);
    } else if (my_strcmp(op, "<<")) {
        printf("%d\n", a << b);
    } else if (my_strcmp(op, ">>")) {
        printf("%d\n", a >> b);
    }
}
