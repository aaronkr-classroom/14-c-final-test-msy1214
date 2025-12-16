#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. Student 구조체 정의
// 요구사항: 이름, 국어, 영어, 수학 점수, 총점, 평균, 등수 포함
typedef struct Student {
    char name[12];
    unsigned short kor; // 국어 점수
    unsigned short eng; // 영어 점수
    unsigned short math; // 수학 점수
    int total;          // 총점
    float average;      // 평균
    int rank;           // 등수
    struct Student* next; // 연결 리스트를 위한 포인터
} Student;

// 전역 변수: 연결 리스트의 시작(헤드)
Student* head = NULL;
// 파일 이름
#define FILENAME "score.dat"

// 함수 프로토타입 선언
void display_menu();
void load_data_from_file();
void input_student_info();
void display_student_info();
void calculate_rank();
void save_data_to_file();
void free_list();

int main() {
    int choice;

    // 프로그램 시작 시 파일에서 데이터 로드
    load_data_from_file();

    while (1) {
        display_menu();
        printf("선택 (1~5): ");
        if (scanf("%d", &choice) != 1) {
            // 입력 오류 처리 (예: 문자 입력)
            while (getchar() != '\n'); // 입력 버퍼 비우기
            choice = 0; // 잘못된 선택으로 처리
        }
        while (getchar() != '\n'); // 나머지 입력 버퍼 비우기

        printf("\n"); // 출력 보기 좋게 개행

        switch (choice) {
        case 1:
            input_student_info();
            break;
        case 2:
            display_student_info();
            break;
        case 3:
            save_data_to_file();
            break;
        case 4:
            calculate_rank();
            break;
        case 5:
            printf("프로그램을 종료합니다.\n");
            save_data_to_file(); // 종료 전 최종 저장
            free_list();         // 메모리 해제
            return 0;
        default:
            printf("잘못된 선택입니다. 1에서 5 사이의 숫자를 입력해주세요.\n");
            break;
        }
    }

    return 0;
}

// 메뉴 출력 함수
void display_menu() {
    printf("\n--- [Menu] ---\n");
    printf("1. 성적 정보 입력\n");
    printf("2. 성적 확인 (출력)\n");
    printf("3. 파일에 저장 (score.dat)\n");
    printf("4. 등수 계산\n");
    printf("5. 종료\n");
    printf("--------------\n");
}

// 1. 성적 정보 입력 함수
void input_student_info() {
    Student* newNode = (Student*)malloc(sizeof(Student));
    if (newNode == NULL) {
        printf("메모리 할당 실패!\n");
        return;
    }

    printf("--- [성적 정보 입력] ---\n");
    printf("이름: ");
    if (scanf("%11s", newNode->name) != 1) { // 최대 11자 + 널 문자 = 12
        printf("이름 입력 오류.\n");
        free(newNode);
        return;
    }

    // 힌트 4: scanf 사용
    printf("국어 점수: ");
    scanf("%hu", &newNode->kor);
    printf("영어 점수: ");
    scanf("%hu", &newNode->eng);
    printf("수학 점수: ");
    scanf("%hu", &newNode->math);

    // 총점 및 평균 계산
    newNode->total = newNode->kor + newNode->eng + newNode->math;
    // (float) 3.0f로 나누어 실수 평균 계산
    newNode->average = (float)newNode->total / 3.0f;
    newNode->rank = 1; // 초기 등수는 1로 설정

    // 새로운 노드를 연결 리스트의 맨 앞에 추가 (head에 삽입)
    newNode->next = head;
    head = newNode;

    // 힌트 1: '1' 선택 시 점수를 입력하고 Menu 화면으로 돌아감
    printf("\n>>> %s 학생의 성적 정보가 입력되었습니다. (총점: %d, 평균: %.2f)\n", newNode->name, newNode->total, newNode->average);
}


// 2. 성적 확인 (출력) 함수
void display_student_info() {
    Student* current = head;

    if (current == NULL) {
        printf(">>> 출력할 학생 정보가 없습니다.\n");
        return;
    }

    // 힌트 4: 출력 시 print 함수 사용
    printf("--- [성적 확인] ---\n");
    printf("%-10s %-4s %-4s %-4s %-6s %-6s %-4s\n",
        "이름", "국어", "영어", "수학", "총점", "평균", "등수");
    printf("--------------------------------------------------\n");

    while (current != NULL) {
        printf("%-10s %-4hu %-4hu %-4hu %-6d %-6.2f %-4d\n",
            current->name, current->kor, current->eng, current->math,
            current->total, current->average, current->rank);
        current = current->next;
    }
    // 힌트 2: '2' 선택 시 정보를 출력하고 Menu 화면으로 돌아감
}

// 3. 현재 데이터를 이진 파일에 저장 함수
void save_data_to_file() {
    FILE* fp = fopen(FILENAME, "wb"); // 이진 쓰기 모드
    if (fp == NULL) {
        printf(">>> 파일 쓰기 오류: %s\n", FILENAME);
        return;
    }

    Student* current = head;
    int count = 0;
    while (current != NULL) {
        // next 포인터 필드를 제외하고 데이터 저장
        // Student 구조체의 크기에서 next 포인터 크기를 뺀 만큼 저장
        // sizeof(Student) - sizeof(Student*) 대신, 필요한 필드만 포함하는 임시 구조체 사용이 더 안전
        struct {
            char name[12];
            unsigned short kor;
            unsigned short eng;
            unsigned short math;
            int total;
            float average;
            int rank;
        } temp;

        strcpy(temp.name, current->name);
        temp.kor = current->kor;
        temp.eng = current->eng;
        temp.math = current->math;
        temp.total = current->total;
        temp.average = current->average;
        temp.rank = current->rank;

        fwrite(&temp, sizeof(temp), 1, fp);
        current = current->next;
        count++;
    }

    fclose(fp);
    printf(">>> 현재 학생 정보 %d개가 %s 파일에 이진 형식으로 저장되었습니다.\n", count, FILENAME);
}

// 프로그램 시작 시 파일에서 데이터 로드 함수
void load_data_from_file() {
    FILE* fp = fopen(FILENAME, "rb"); // 이진 읽기 모드
    if (fp == NULL) {
        printf(">>> %s 파일이 없거나 열 수 없습니다. 새 데이터로 시작합니다.\n", FILENAME);
        return;
    }

    // 파일 로드 전에 기존 리스트 데이터 메모리 해제
    free_list();
    head = NULL;

    struct {
        char name[12];
        unsigned short kor;
        unsigned short eng;
        unsigned short math;
        int total;
        float average;
        int rank;
    } temp;

    int count = 0;

    // 파일 끝까지 반복하며 데이터를 읽어 리스트에 추가
    while (fread(&temp, sizeof(temp), 1, fp) == 1) {
        Student* newNode = (Student*)malloc(sizeof(Student));
        if (newNode == NULL) {
            printf("메모리 할당 실패!\n");
            break;
        }

        // 임시 구조체의 데이터를 새 노드에 복사
        strcpy(newNode->name, temp.name);
        newNode->kor = temp.kor;
        newNode->eng = temp.eng;
        newNode->math = temp.math;
        newNode->total = temp.total;
        newNode->average = temp.average;
        newNode->rank = temp.rank;

        // 노드를 리스트 맨 앞에 추가 (읽는 순서대로 리스트에 추가됨)
        newNode->next = head;
        head = newNode;
        count++;
    }

    fclose(fp);
    printf(">>> %s 파일에서 학생 정보 %d개를 성공적으로 로드했습니다.\n", FILENAME, count);
}

// 4. 등수 계산 함수
void calculate_rank() {
    Student* i, * j;
    int count = 0;

    if (head == NULL) {
        printf(">>> 계산할 학생 정보가 없습니다.\n");
        return;
    }

    // 1단계: 모든 학생의 등수를 1로 초기화
    i = head;
    while (i != NULL) {
        i->rank = 1;
        i = i->next;
        count++;
    }

    // 2단계: 등수 계산
    i = head;
    while (i != NULL) {
        j = head;
        while (j != NULL) {
            // 총점을 기준으로 비교
            if (i->total < j->total) {
                i->rank++;
            }
            j = j->next;
        }
        i = i->next;
    }

    printf(">>> 총 %d명의 학생의 등수 계산이 완료되었습니다.\n", count);
}

// 리스트 메모리 해제 함수
void free_list() {
    Student* current = head;
    Student* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    head = NULL; // 헤드 포인터를 NULL로 설정하여 안전하게 마무리
}