#include "Common.h"



int Exercise01()
{
	WSADATA wsa;
	MAKEWORD(2, 2);
	int errCode = WSAStartup(0x0202, &wsa);
	if (errCode != 0) {
		err_display(errCode);
	}
	printf("[�˸�] ���� �ʱ�ȭ ����\n");
	int x = htons(wsa.wVersion);
	printf("wVersion: %04x\n", x);
	printf("wHighVersion: %d\n", wsa.wHighVersion);
	printf("szDescription: %s\n", wsa.szDescription);
	printf("szSystemStatus: %s\n", wsa.szSystemStatus);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	printf("[�˸�] ���� ���� ����\n");

	if(closesocket(sock) != 0) {
		err_display("closesocket()");
	}
	if (WSACleanup() != 0) err_quit("WSACleanup()");
	return 0;
}

int Exercise02()
{
	WSADATA wsa;
	int errCode = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (errCode != 0) {
		err_display(errCode);
	}
	
	u_short x1 = 0x1234;
	u_long y1 = 0x12345678;
	u_short x2;
	u_long y2;
	printf("[ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ]\n");
	printf("%#x -> %#x\n", x1, x2 = htons(x1));
	printf("%#x -> %#x\n", y1, y2 = htonl(y1));

	printf("\n[��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ]\n");
	printf("%#x -> %#x\n", x2, ntohs(x2));
	printf("%#x -> %#x\n", y2, ntohl(y2));

	printf("\n[�߸��� ��� ��]\n");
	printf("%#x -> %#x\n", x1, htonl(x1));

	if (WSACleanup() != 0)
		err_display("WSACleanup()");
	return 0;
}

int Exercise03_2()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	
	const char* ipv4test = "147.46.114.70";
	printf("IPv4 �ּ�(��ȯ��) = %s\n", ipv4test);

	struct in_addr ipv4num;
	inet_pton(AF_INET, ipv4test, &ipv4num);
	printf("IPv4 �ּ�(��ȯ��) = %#x\n", ipv4num.s_addr);

	// inet_ntop() �Լ� ����
	char ipv4str[16]; // INET_ADDRSTRLEN ���̰� 22�ε� 16���ε� ����� ���̴µ� 22�� ����
	inet_ntop(AF_INET, &ipv4num, ipv4str, sizeof(ipv4str));
	printf("IPv4 �ּ� (�ٽ� ��ȯ ��) = %s\n", ipv4str);
	printf("\n");
	
	
	WSACleanup();
	return 0;
}


#define TESTNAME "www.google.com"

bool GetIpAddr(const char* name, struct in_addr* addr)
{
	struct hostent* ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_display("gethostbyname()");
		return false;
	}
	if (ptr->h_addrtype != AF_INET)
		return false;
	memcpy(addr, ptr->h_addr, ptr->h_length);
	return true;
}

bool GetDomainName(struct in_addr addr, char* name, int namelen)
{
	struct hostent* ptr = gethostbyaddr((const char*)&addr, sizeof(addr), AF_INET);
	if (ptr == NULL) {
		err_display("gethostbyaddr()");
		return false;
	}
	if (ptr->h_addrtype != AF_INET)
		return false;
	strncpy(name, ptr->h_name, namelen);
	return true;
}
int Exercise04()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	printf("������ �̸�(��ȯ ��) = %s\n", TESTNAME);

	// ������ �̸�->IP �ּ�
	struct in_addr addr;
	if (GetIpAddr(TESTNAME, &addr)) {
		char str[16];
		inet_ntop(AF_INET, &addr, str, sizeof(str));
		printf("IP �ּ�(��ȯ ��) = %s\n", str);

		// IP �ּ� -> ������ ���
		char name[256];
		if (GetDomainName(addr, name, sizeof(name))) {
			printf("������ �̸�(�ٽ� ��ȯ ��) = %s\n", name);
		}
	}
	WSACleanup();
	return 0;
}