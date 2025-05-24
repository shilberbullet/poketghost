#ifndef TEXT_H
#define TEXT_H

// 텍스트를 한 글자씩 출력하는 함수
void printText(const char* text);

// 텍스트 출력 후 엔터키를 기다리는 함수
void printTextAndWait(const char* text);

// 텍스트 출력 속도에 따른 대기 시간 계산 함수
int getTextDelay();

#endif // TEXT_H 