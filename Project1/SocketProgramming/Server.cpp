#include "Common.h"

int main()
{
	WSADATA wsa;
	if (WSAStartup(0x22, &wsa) != 0)
		return 1;
	printf("[�˸�] ���� �ʱ�ȭ ����\n");


	WSACleanup();
	return 0;
}