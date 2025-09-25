#include "Common.h"

int Exercise02_1()
{
	WSADATA wsa;
	MAKEWORD(2, 2);
	int errCode = WSAStartup(0x0202, &wsa);
	if (errCode != 0) {
		err_display(errCode);
	}
	printf("[알림] 윈속 초기화 성공\n");
	int x = htons(wsa.wVersion);
	printf("wVersion: %04x\n", x);
	printf("wHighVersion: %d\n", wsa.wHighVersion);
	printf("szDescription: %s\n", wsa.szDescription);
	printf("szSystemStatus: %s\n", wsa.szSystemStatus);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	printf("[알림] 소켓 생성 성공\n");

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
	printf("[알림] 윈속 초기화 성공\n");

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	printf("[알림] 소켓 생성 성공\n");

	closesocket(sock);

	WSACleanup();
	return 0;
}
int Exercise02_3()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	printf("[알림] 윈속 초기화 성공\n");

	SOCKET sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	printf("[알림] 소켓 생성 성공\n");

	closesocket(sock);

	WSACleanup();
	return 0;
}
int Exercise02_4()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	printf("[알림] 윈속 초기화 성공\n");

	SOCKET sock = socket(AF_INET6, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	printf("[알림] 소켓 생성 성공\n");

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
	printf("윈속 초기화 성공\n");
	SOCKET sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET)
		err_display("WSASocket()");
	printf("소켓 생성 성공\n");
	closesocket(sock);
	WSACleanup();
	return 0;
}




int Example02() // 예제
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
	printf("[호스트 바이트 -> 네트워크 바이트]\n");
	printf("%#x -> %#x\n", x1, x2 = htons(x1));
	printf("%#x -> %#x\n", y1, y2 = htonl(y1));

	printf("\n[네트워크 바이트 -> 호스트 바이트]\n");
	printf("%#x -> %#x\n", x2, ntohs(x2));
	printf("%#x -> %#x\n", y2, ntohl(y2));

	printf("\n[잘못된 사용 예]\n");
	printf("%#x -> %#x\n", x1, htonl(x1));

	if (WSACleanup() != 0)
		err_display("WSACleanup()");
	return 0;
}


bool IsLittileEndian03_01()
{
	// int i = 1을 선언하고 저장 방식을 확인
	// 빅엔디안이면 0x00 00 00 01으로 저장
	// 리틀엔디안이면 0x 01 00 00 00으로 저장
	int i = 1;
	//i를 1바이트 크기로 메모리에 접근
	char* c = (char*)&i;

	// 맨 앞 바이트가 1인지 체크, 1이면 리틀엔디안
	if (*c == 1) {
		return true;
	}
	else {
		return false;
	}
}
bool IsLittileEndian03_01_v2()
{
	// 꼭 어렵게 포인터로 접근해서 리틀엔디안인지 확인해야할까?
	// 더 좋은방법은 없는지
	
	// 우선 리틀엔디안인지 체크하는건 2byte면 충분하다
	short i = 1;
	// 그리고 HIBYTE 매크로 함수로 맨앞 1바이트를 체크한다.
	BYTE a = HIBYTE(i);
	if (a == (BYTE)0x01) {
		return true;
	}
	else {
		return false;
	}
	// 그런데 안된다. 이유 ? 논리적인 값으로 변환해서?
}
bool IsLittileEndian03_01_v3()
{
	// 꼭 어렵게 포인터로 접근해서 리틀엔디안인지 확인해야할까?
	// 더 좋은방법은 없는지

	// 우선 리틀엔디안인지 체크하는건 2byte면 충분하다
	short i = 1;
	// 그리고 HIBYTE 매크로 함수로 맨앞 1바이트를 체크한다.
	if (htons(i)) { // network 주소 체계 변환 시도
		return true; // 성공했으면 리틀엔디안
	}
	else {
		return false;
	}
	// 그런데 안된다. 이유 ? 논리적인 값으로 변환해서?
}
void checkEndian03_1()
{
	if (IsLittileEndian03_01_v3()) {
		printf("시스템은 Littile-Endian 방식을 사용합니다.\n");
	}
	else {
		printf("시스템은 Big-Endian 방식을 사용합니다.\n");
	}
}
int Exercise03_1()
{
	printf("시스템의 바이트 정렬 방식을 확인합니다.\n");
	checkEndian03_1();
	return 0;
}

int Exercise03_2()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	const char* ipv4test = "147.46.114.70";
	printf("IPv4 주소(변환전) = %s\n", ipv4test);

	unsigned long retval = inet_addr(ipv4test);
	if (retval == INADDR_ANY || retval == INADDR_NONE) {
		WSASetLastError(WSAEADDRNOTAVAIL);
		err_display("inet_addr()");
	}

	struct in_addr ipv4num;
	ipv4num.S_un.S_addr = retval;

	printf("IPv4 주소(변환후) = %#x\n", ipv4num.s_addr);
	
	printf("IPv4 주소(변환후) 십진수로 = %d.%d.%d.%d\n", ipv4num.S_un.S_un_b.s_b1,
		ipv4num.S_un.S_un_b.s_b2, ipv4num.S_un.S_un_b.s_b3, ipv4num.S_un.S_un_b.s_b4);
	printf("IPv4 주소(변환후) 십진수로(v2) = %d.%d.%d.%d\n", HIBYTE(ipv4num.S_un.S_un_w.s_w1), 
		LOBYTE(ipv4num.S_un.S_un_w.s_w1), HIBYTE(ipv4num.S_un.S_un_w.s_w2),
		LOBYTE(ipv4num.S_un.S_un_w.s_w2));
	printf("IPv4 주소(변환후) 십진수로(v3) = %d.%d.%d.%d\n", HIBYTE(ipv4num.S_un.S_addr),
		LOBYTE(HIWORD(ipv4num.S_un.S_addr)), HIBYTE(LOWORD(ipv4num.S_un.S_addr)),
		LOBYTE(ipv4num.S_un.S_addr));
	printf("IPv4 주소(변환후) 십진수로(v4) = %d.%d.%d.%d\n", HIBYTE(HIWORD(ipv4num.S_un.S_addr)),
		LOBYTE(HIWORD(ipv4num.S_un.S_addr)), HIBYTE(LOWORD(ipv4num.S_un.S_addr)),
		LOBYTE(ipv4num.S_un.S_addr));


	char ipv4str[16]; // INET_ADDRSTRLEN 길이가 22인데 16으로도 충분해 보이는데 22인 이유
	inet_ntop(AF_INET, &ipv4num, ipv4str, sizeof(ipv4str));
	printf("IPv4 주소 (다시 변환 후) = %s\n", ipv4str);
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
	printf("IPv4 주소(변환전) = %s\n", ipv4test);

	struct in_addr ipv4num;
	inet_pton(AF_INET, ipv4test, &ipv4num);
	printf("IPv4 주소(변환후) = %#x\n", ipv4num.s_addr);
	// inet_ntoa()로 반환된 문자열 = 소켓에 의해 할당된 메모리, 유효값 보장 - 동일 스레드 내에 소켓 함수 호출 전까지
	char ipv4str[16];
	char* a = inet_ntoa(ipv4num); 
	// ipv4num 
	printf("IPv4 주소 (다시 변환 후) = %s\n", a);
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
	printf("IPv4 주소(변환전) = %s\n", ipv4test);
	sockaddr_in addr;
	int addrlen = sizeof(addr);
	
	if (WSAStringToAddressA(ipv4test, AF_INET, NULL, (sockaddr*)&addr, &addrlen) == SOCKET_ERROR)
	{
		err_display("WSAStringToAddressA()");
	}


	printf("IPv4 주소(변환후) = %#x\n", addr.sin_addr.S_un.S_addr);
	// inet_ntoa()로 반환된 문자열 = 소켓에 의해 할당된 메모리, 유효값 보장 - 동일 스레드 내에 소켓 함수 호출 전까지
	char ipv4str[16];
	char* a = inet_ntoa(addr.sin_addr);
	// ipv4num 
	printf("IPv4 주소 (다시 변환 후) = %s\n", a);
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
	printf("IPv4 주소(변환전) = %s\n", ipv4test);
	sockaddr_in addr;
	int addrlen = sizeof(addr);

	if (WSAStringToAddressA(ipv4test, AF_INET, NULL, (sockaddr*)&addr, &addrlen) == SOCKET_ERROR)
	{
		err_display("WSAStringToAddressA()");
	}


	printf("IPv4 주소(변환후) = %#x\n", addr.sin_addr.S_un.S_addr);

	char ipv4str[16];
	DWORD ipv4str_len = sizeof(ipv4str);
	if (WSAAddressToStringA((sockaddr*)&addr, addrlen, NULL, ipv4str, &ipv4str_len) == SOCKET_ERROR) {
		err_display("WSAAddressToStringA()");
	}
	// ipv4num 
	printf("IPv4 주소 (다시 변환 후) = %s\n", ipv4str);
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
	printf("IPv4 주소(변환전) = %s\n", ipv4test);

	struct in_addr ipv4num;
	inet_pton(AF_INET, ipv4test, &ipv4num);
	printf("IPv4 주소(변환후) = %#x\n", ipv4num.s_addr);

	// inet_ntop() 함수 연습
	char ipv4str[16]; // INET_ADDRSTRLEN 길이가 22인데 16으로도 충분해 보이는데 22인 이유
	inet_ntop(AF_INET, &ipv4num, ipv4str, sizeof(ipv4str));
	printf("IPv4 주소 (다시 변환 후) = %s\n", ipv4str);
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

	printf("도메인 이름(변환 전) = %s\n", TESTNAME);

	// 도메인 이름->IP 주소
	struct in_addr addr;
	if (GetIpAddr(TESTNAME, &addr)) {
		char str[16];
		inet_ntop(AF_INET, &addr, str, sizeof(str));
		printf("IP 주소(변환 후) = %s\n", str);

		// IP 주소 -> 도메인 출력
		char name[256];
		if (GetDomainName(addr, name, sizeof(name))) {
			printf("도메인 이름(다시 변환 후) = %s\n", name);
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