#include "..\Common.h"

int main(int argc, char* argv[])
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	int major_version = 2;
	int minor_version = 2;
	WORD version_requested = (WORD)(((BYTE)(((DWORD_PTR)(major_version)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(minor_version)) & 0xff))) << 8);
	if (WSAStartup(version_requested, &wsa) != 0)
		return 1;
	printf("[�˸�] ���� �ʱ�ȭ ����\n");
	printf("wVersion: %d.%d\n", LOBYTE(wsa.wVersion), HIBYTE(wsa.wVersion));
	printf("wHighVersion: %d.%d\n", LOBYTE(wsa.wHighVersion), HIBYTE(wsa.wVersion));
	printf("szDescription: %s\n", wsa.szDescription);
	printf("szSystemStatus: %s\n", wsa.szSystemStatus);

	// ���� ����
	WSACleanup();
	return 0;
}

// HIBYTE, LOBYTE ��ũ�δ� ���� WORD Ÿ���� ������� ���� ����Ʈ�� ���� ����Ʈ�� �����ϴ� �� ���ȴ�.