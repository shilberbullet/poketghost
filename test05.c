#include <stdio.h>
#include <string.h>

void arithmetic();
void bitwise();

int main() {
    int choice;

    while (1) {
        printf("=== 계산기 프로그램 ===\n");
        printf("1. 산술 연산\n");
        printf("2. 비트 연산\n");
        printf("3. 종료\n");
        printf("선택: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("잘못된 입력입니다! 다시 시도하세요.\n");
            while (getchar() != '\n'); // 입력 버퍼 비우기
            continue;
        }

        if (choice == 1) {
            arithmetic();
        } else if (choice == 2) {
            bitwise();
        } else if (choice == 3) {
            printf("프로그램 종료!\n");
            break;
        } else {
            printf("잘못된 입력입니다! 다시 시도하세요.\n");
        }
    }

    return 0;
}

void arithmetic() {
    char op;
    int a, b;
    int valid = 0;

    while (!valid) {
        printf("[+] [-] [*] [/] [%] 선택: ");
        scanf(" %c", &op);  // 앞에 공백을 두어 이전 입력 버퍼 제거

        if (op == '+' || op == '-' || op == '*' || op == '/' || op == '%') {
            valid = 1;
        } else {
            printf("잘못된 입력입니다! 다시 시도하세요.\n");
        }
    }

    while (1) {
        printf("숫자 두 개 입력: ");
        if (scanf("%d %d", &a, &b) != 2) {
            printf("잘못된 입력입니다! 다시 시도하세요.\n");
            while (getchar() != '\n');
            continue;
        }

        if ((op == '/' || op == '%') && b == 0) {
            printf("0으로 나눌 수 없습니다! 다시 시도하세요.\n");
            continue;
        }

        break;
    }

    printf("결과: ");
    switch (op) {
        case '+': printf("%d\n", a + b); break;
        case '-': printf("%d\n", a - b); break;
        case '*': printf("%d\n", a * b); break;
        case '/': printf("%d\n", a / b); break;
        case '%': printf("%d\n", a % b); break;
    }
}

void bitwise() {
    char op[3];
    int a, b;
    int valid = 0;

    while (!valid) {
        printf("[&] [|] [^] [<<] [>>] 선택: ");
        scanf("%s", op);

        if (strcmp(op, "&") == 0 || strcmp(op, "|") == 0 || strcmp(op, "^") == 0 ||
            strcmp(op, "<<") == 0 || strcmp(op, ">>") == 0) {
            valid = 1;
        } else {
            printf("잘못된 입력입니다! 다시 시도하세요.\n");
        }
    }

    while (1) {
        printf("숫자 두 개 입력: ");
        if (scanf("%d %d", &a, &b) != 2) {
            printf("잘못된 입력입니다! 다시 시도하세요.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    }

    printf("결과: ");
    if (strcmp(op, "&") == 0) {
        printf("%d\n", a & b);
    } else if (strcmp(op, "|") == 0) {
        printf("%d\n", a | b);
    } else if (strcmp(op, "^") == 0) {
        printf("%d\n", a ^ b);
    } else if (strcmp(op, "<<") == 0) {
        printf("%d\n", a << b);
    } else if (strcmp(op, ">>") == 0) {
        printf("%d\n", a >> b);
    }
}




