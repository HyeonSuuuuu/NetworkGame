#include "Exercise.h"
#include "Common.h"
#include <conio.h>
int (*pf[100])() = {Exercise01, Exercise02};
int pf_count = 2;

int main()
{
	while (1) {
		system("cls");
		printf("----------------------------------------------\n");
		printf("1: Exercise01 실행\n");
		printf("2: Exercise02 실행\n");
		printf("----------------------------------------------\n");

		
		int input;
		if (scanf("%d", &input) != 1) {
			return 0;
		}
		
		if (input > 0 && input <= pf_count) {
			system("cls");
			pf[input - 1]();

			_getch();
		}
	}
}


