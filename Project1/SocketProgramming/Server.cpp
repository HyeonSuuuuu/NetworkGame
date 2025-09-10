#include "Common.h"

int main()
{
	WSADATA wsa;
	if (WSAStartup(0x22, &wsa) != 0)
		return 1;
	printf("[알림] 윈속 초기화 성공\n");


	WSACleanup();
	return 0;
}