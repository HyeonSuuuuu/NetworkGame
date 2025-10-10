#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include "Exercise.cpp"

// Exercise.cpp 빌드에서 제외 시킬것

//#define TEST
//#define MAIN

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

int main(int argc, char** argv)
{
	if (argc != 2) {
		return 0;
	}

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	hostent* ptr = gethostbyname(argv[1]);
	if (ptr == NULL) {
		err_quit("gethostbyname()");
	}
	if (ptr->h_addrtype != AF_INET)
		return 0;
	printf("공식 도메인 이름: %s\n", ptr->h_name);
	for (int i = 0; ptr->h_aliases[i] != NULL; i++) {
		printf("별칭 도메인 이름[%d]: %s\n", i + 1, ptr->h_aliases[i]);
	}
	in_addr** addr_list = (in_addr**)ptr->h_addr_list;
	for (int i = 0; ptr->h_addr_list[i] != NULL; i++) {
		in_addr* ipv4addr = addr_list[i];
		char* ipv4str = inet_ntoa(*ipv4addr);
		printf("IPv4 주소[%d] : %s\n", i + 1, ipv4str);
	}

	WSACleanup();
	return 0;
}
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
Example02,
Example03_2,
Example03_3, 
Example04_01_s,//15
Example04_01_c,
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
		
		int c;
		while ((c = getchar()) != '\n' && c != EOF);


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

