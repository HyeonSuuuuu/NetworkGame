#include "Common.h"

int Exercise02_1()
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
int Exercise02_2()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	printf("[�˸�] ���� �ʱ�ȭ ����\n");

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	printf("[�˸�] ���� ���� ����\n");

	closesocket(sock);

	WSACleanup();
	return 0;
}
int Exercise02_3()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	printf("[�˸�] ���� �ʱ�ȭ ����\n");

	SOCKET sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	printf("[�˸�] ���� ���� ����\n");

	closesocket(sock);

	WSACleanup();
	return 0;
}
int Exercise02_4()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	printf("[�˸�] ���� �ʱ�ȭ ����\n");

	SOCKET sock = socket(AF_INET6, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	printf("[�˸�] ���� ���� ����\n");

	closesocket(sock);
	WSACleanup();
	return 0;
}

int f02_5(int x)
{
	if (x >= 0) {
		WSASetLastError(0);
		return 0;
	}
		
	if (x < 0) {
		WSASetLastError(WSAEINVAL);
		return SOCKET_ERROR;
	}
		
}

int Exercise02_5()
{
	int retval = f02_5(100);
	if (retval == SOCKET_ERROR)
		err_quit("f02_5()");
	return 0;
}

int Exercise02_6()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 1;
	}
	printf("���� �ʱ�ȭ ����\n");
	SOCKET sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET)
		err_display("WSASocket()");
	printf("���� ���� ����\n");
	closesocket(sock);
	WSACleanup();
	return 0;
}




int Example02() // ����
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


bool IsLittileEndian03_01()
{
	// int i = 1�� �����ϰ� ���� ����� Ȯ��
	// �򿣵���̸� 0x00 00 00 01���� ����
	// ��Ʋ������̸� 0x 01 00 00 00���� ����
	int i = 1;
	//i�� 1����Ʈ ũ��� �޸𸮿� ����
	char* c = (char*)&i;

	// �� �� ����Ʈ�� 1���� üũ, 1�̸� ��Ʋ�����
	if (*c == 1) {
		return true;
	}
	else {
		return false;
	}
}
bool IsLittileEndian03_01_v2()
{
	// �� ��ư� �����ͷ� �����ؼ� ��Ʋ��������� Ȯ���ؾ��ұ�?
	// �� ��������� ������
	
	// �켱 ��Ʋ��������� üũ�ϴ°� 2byte�� ����ϴ�
	short i = 1;
	// �׸��� HIBYTE ��ũ�� �Լ��� �Ǿ� 1����Ʈ�� üũ�Ѵ�.
	BYTE a = HIBYTE(i);
	if (a == (BYTE)0x01) {
		return true;
	}
	else {
		return false;
	}
	// �׷��� �ȵȴ�. ���� ? ������ ������ ��ȯ�ؼ�?
}
bool IsLittileEndian03_01_v3()
{
	// �� ��ư� �����ͷ� �����ؼ� ��Ʋ��������� Ȯ���ؾ��ұ�?
	// �� ��������� ������

	// �켱 ��Ʋ��������� üũ�ϴ°� 2byte�� ����ϴ�
	short i = 1;
	// �׸��� HIBYTE ��ũ�� �Լ��� �Ǿ� 1����Ʈ�� üũ�Ѵ�.
	if (htons(i)) { // network �ּ� ü�� ��ȯ �õ�
		return true; // ���������� ��Ʋ�����
	}
	else {
		return false;
	}
	// �׷��� �ȵȴ�. ���� ? ������ ������ ��ȯ�ؼ�?
}
void checkEndian03_1()
{
	if (IsLittileEndian03_01_v3()) {
		printf("�ý����� Littile-Endian ����� ����մϴ�.\n");
	}
	else {
		printf("�ý����� Big-Endian ����� ����մϴ�.\n");
	}
}
int Exercise03_1()
{
	printf("�ý����� ����Ʈ ���� ����� Ȯ���մϴ�.\n");
	checkEndian03_1();
	return 0;
}

int Exercise03_2()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	const char* ipv4test = "147.46.114.70";
	printf("IPv4 �ּ�(��ȯ��) = %s\n", ipv4test);

	unsigned long retval = inet_addr(ipv4test);
	if (retval == INADDR_ANY || retval == INADDR_NONE) {
		WSASetLastError(WSAEADDRNOTAVAIL);
		err_display("inet_addr()");
	}

	struct in_addr ipv4num;
	ipv4num.S_un.S_addr = retval;

	printf("IPv4 �ּ�(��ȯ��) = %#x\n", ipv4num.s_addr);
	
	printf("IPv4 �ּ�(��ȯ��) �������� = %d.%d.%d.%d\n", ipv4num.S_un.S_un_b.s_b1,
		ipv4num.S_un.S_un_b.s_b2, ipv4num.S_un.S_un_b.s_b3, ipv4num.S_un.S_un_b.s_b4);
	printf("IPv4 �ּ�(��ȯ��) ��������(v2) = %d.%d.%d.%d\n", HIBYTE(ipv4num.S_un.S_un_w.s_w1), 
		LOBYTE(ipv4num.S_un.S_un_w.s_w1), HIBYTE(ipv4num.S_un.S_un_w.s_w2),
		LOBYTE(ipv4num.S_un.S_un_w.s_w2));
	printf("IPv4 �ּ�(��ȯ��) ��������(v3) = %d.%d.%d.%d\n", HIBYTE(ipv4num.S_un.S_addr),
		LOBYTE(HIWORD(ipv4num.S_un.S_addr)), HIBYTE(LOWORD(ipv4num.S_un.S_addr)),
		LOBYTE(ipv4num.S_un.S_addr));
	printf("IPv4 �ּ�(��ȯ��) ��������(v4) = %d.%d.%d.%d\n", HIBYTE(HIWORD(ipv4num.S_un.S_addr)),
		LOBYTE(HIWORD(ipv4num.S_un.S_addr)), HIBYTE(LOWORD(ipv4num.S_un.S_addr)),
		LOBYTE(ipv4num.S_un.S_addr));


	char ipv4str[16]; // INET_ADDRSTRLEN ���̰� 22�ε� 16���ε� ����� ���̴µ� 22�� ����
	inet_ntop(AF_INET, &ipv4num, ipv4str, sizeof(ipv4str));
	printf("IPv4 �ּ� (�ٽ� ��ȯ ��) = %s\n", ipv4str);
	printf("\n");
	WSACleanup();
	return 0;
}

int Exercise03_3()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	const char* ipv4test = "147.46.114.70";
	printf("IPv4 �ּ�(��ȯ��) = %s\n", ipv4test);

	struct in_addr ipv4num;
	inet_pton(AF_INET, ipv4test, &ipv4num);
	printf("IPv4 �ּ�(��ȯ��) = %#x\n", ipv4num.s_addr);
	// inet_ntoa()�� ��ȯ�� ���ڿ� = ���Ͽ� ���� �Ҵ�� �޸�, ��ȿ�� ���� - ���� ������ ���� ���� �Լ� ȣ�� ������
	char ipv4str[16];
	char* a = inet_ntoa(ipv4num); 
	// ipv4num 
	printf("IPv4 �ּ� (�ٽ� ��ȯ ��) = %s\n", a);
	printf("\n");


	WSACleanup();
	return 0;
}
int Exercise03_4()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	char ipv4test[] = "147.46.114.70";
	printf("IPv4 �ּ�(��ȯ��) = %s\n", ipv4test);
	sockaddr_in addr;
	int addrlen = sizeof(addr);
	
	if (WSAStringToAddressA(ipv4test, AF_INET, NULL, (sockaddr*)&addr, &addrlen) == SOCKET_ERROR)
	{
		err_display("WSAStringToAddressA()");
	}


	printf("IPv4 �ּ�(��ȯ��) = %#x\n", addr.sin_addr.S_un.S_addr);
	// inet_ntoa()�� ��ȯ�� ���ڿ� = ���Ͽ� ���� �Ҵ�� �޸�, ��ȿ�� ���� - ���� ������ ���� ���� �Լ� ȣ�� ������
	char ipv4str[16];
	char* a = inet_ntoa(addr.sin_addr);
	// ipv4num 
	printf("IPv4 �ּ� (�ٽ� ��ȯ ��) = %s\n", a);
	printf("\n");

	WSACleanup();
	return 0;
}
int Exercise03_5()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	char ipv4test[] = "147.46.114.110";
	printf("IPv4 �ּ�(��ȯ��) = %s\n", ipv4test);
	sockaddr_in addr;
	int addrlen = sizeof(addr);

	if (WSAStringToAddressA(ipv4test, AF_INET, NULL, (sockaddr*)&addr, &addrlen) == SOCKET_ERROR)
	{
		err_display("WSAStringToAddressA()");
	}


	printf("IPv4 �ּ�(��ȯ��) = %#x\n", addr.sin_addr.S_un.S_addr);

	char ipv4str[16];
	DWORD ipv4str_len = sizeof(ipv4str);
	if (WSAAddressToStringA((sockaddr*)&addr, addrlen, NULL, ipv4str, &ipv4str_len) == SOCKET_ERROR) {
		err_display("WSAAddressToStringA()");
	}
	// ipv4num 
	printf("IPv4 �ּ� (�ٽ� ��ȯ ��) = %s\n", ipv4str);
	printf("\n");

	WSACleanup();
	return 0;
}
int Example03_2()
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
int Example04()
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

int Example04_01()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	SOCKET server_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if (server_sock == INVALID_SOCKET)
		err_quit("socket()");

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(server_sock, (sockaddr*)&addr, sizeof(addr));
	
	listen(server_sock, SOMAXCONN);

	while (true) {
		SOCKET client_sock;
		sockaddr_in client_addr;
		int addrlen = sizeof(client_addr);
		client_sock = accept(server_sock, (sockaddr*)&client_addr, &addrlen);


	}

	WSACleanup();
	return 0;
}