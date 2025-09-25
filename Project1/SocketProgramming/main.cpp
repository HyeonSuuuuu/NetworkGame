#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include "Exercise.cpp"

// Exercise.cpp ���忡�� ���� ��ų��

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
	printf("���� �ð�: %lld us\n", std::chrono::duration_cast<std::chrono::microseconds>(exec_t).count());
}

#else


int (*pf[100])() = {
Exercise02_1, // 1
Exercise02_2,
Exercise02_3,
Exercise02_4,
Exercise02_5, // 5
Exercise02_6,
Exercise03_1,
Exercise03_2,
Example02,
Example03_2, // 10
Example04,
NULL};

int main()
{
	while (1) {
		system("cls");
		printf("----------------------------------------------\n");
		printf("n = n��° Exercise �Լ� ����\n");
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

