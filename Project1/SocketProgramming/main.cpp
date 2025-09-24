#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include "Exercise.cpp"

// Exercise.cpp 빌드에서 제외 시킬것

// #define TEST

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

#else


int (*pf[100])() = {Exercise01, Exercise02, Exercise03_2, Exercise04
		, NULL};

int main()
{
	while (1) {
		system("cls");
		printf("----------------------------------------------\n");
		printf("n = n번째 Exercise 함수 실행\n");
		printf("----------------------------------------------\n");

		
		int input;
		if (scanf("%d", &input) != 1) {
			return 0;
		}
		
		if (input > 0 && input < 100) {
			system("cls");
			if (pf[input - 1] != NULL)
				pf[input - 1]();
			else
				continue;
			_getch();
		}
	}
}
#endif

