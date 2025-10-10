#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include <conio.h>
#include "Exercise.cpp"

// Exercise.cpp 빌드에서 제외 시킬것

//#define TEST
// #define MAIN

#ifdef TEST
#include <chrono>


int main()
{
	auto start_t = std::chrono::system_clock::now();
//----------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------
	auto end_t = std::chrono::system_clock::now();
	auto exec_t = end_t - start_t;
	printf("실행 시간: %lld us\n", std::chrono::duration_cast<std::chrono::microseconds>(exec_t).count());
}

#elif defined(MAIN)




#else
// 인수로 입력받는거 없이 실행하는 경우
int (*pf[100])() = {
Exercise02_1, // 1
Exercise02_2,
Exercise02_3,
Exercise02_4,
Exercise02_5, // 5
Exercise02_6,
Exercise03_1,
Exercise03_2,
Exercise03_3,
Exercise03_4, // 10
Exercise03_5,
Exercise05_05_s, // 12
Exercise05_05_c, // 13
Exercise05_s,
Exercise05_c,
Example02, // 16 ~
Example03_2,
Example03_3,
Example04_01_s,
Example04_01_c,
Example05_01_s,
Example05_01_c,
Example05_02_s,
Example05_02_c,
Example05_03_s,
Example05_03_c,
Example05_04_s,
Example05_04_c,
Example06_1,
Example06_2,
Example06_3,
NULL};
// 명령행 인수로 입력받아 실행하는 경우
int (*pf_argv[100])(char**) = {
Exercise03_6,
Exercise03_7,
Exercise03_8,
Exercise03_9,
NULL
};

int main(int argc, char** argv)
{
	bool isArgs = false;
	if (argc > 1) {
		isArgs = true;
	}
	while (1) {
		system("cls");
		if (isArgs)
		{
			printf("----------------------------------------------\n");
			printf("명령행 인수로 실행되었습니다.\n");
			printf("n = n번째 Exercise 함수 실행\n");
			printf("기본 모드로 : -1\n");
			printf("----------------------------------------------\n");
		}
		else
		{
			printf("----------------------------------------------\n");
			printf("n = n번째 Exercise 함수 실행\n");
			printf("명령행 인수로 실행: %s <input>\n", argv[0]);
			printf("----------------------------------------------\n");
		}


		int input;
		if (scanf("%d", &input) != 1)
		{
			return 0;
		}
		

		if (input > 0 && input < 100)
		{
			system("cls");
			if (isArgs) {
				if (pf_argv[input - 1] != NULL) {
					pf_argv[input - 1](argv);
				}
			}
			else {
				if (pf[input - 1] != NULL) {
					pf[input - 1]();
				}
			}
			printf("실행 종료");
			_getch();
		}
		else if (isArgs && input == -1)
		{
				isArgs = false;
		}
		else
		{
			return 0;
		}

		
		
	}
}
#endif

