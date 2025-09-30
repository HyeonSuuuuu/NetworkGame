#include "..\Common.h"

int main(int argc, char* argv[])
{
	// 윈속 초기화
	WSADATA wsa;
	int major_version = 2;
	int minor_version = 2;
	WORD version_requested = (WORD)(((BYTE)(((DWORD_PTR)(major_version)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(minor_version)) & 0xff))) << 8);
	if (WSAStartup(version_requested, &wsa) != 0)
		return 1;
	printf("[알림] 윈속 초기화 성공\n");
	printf("wVersion: %d.%d\n", LOBYTE(wsa.wVersion), HIBYTE(wsa.wVersion));
	printf("wHighVersion: %d.%d\n", LOBYTE(wsa.wHighVersion), HIBYTE(wsa.wVersion));
	printf("szDescription: %s\n", wsa.szDescription);
	printf("szSystemStatus: %s\n", wsa.szSystemStatus);

	// 윈속 종료
	WSACleanup();
	return 0;
}

// HIBYTE, LOBYTE 매크로는 각각 WORD 타입의 상수에서 상위 바이트와 하위 바이트를 추출하는 데 사용된다.