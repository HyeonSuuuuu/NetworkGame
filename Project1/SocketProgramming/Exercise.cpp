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
	// inet_ntoa()로 반환된 문자열 = 라이브러리내의 정적데이터에 할당, 유효값 보장 - 어떤스레드든 동일 함수 호출 전까지
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
	char* a = inet_ntoa(addr.sin_addr); // 네트워크바이트 정렬된 주소를 문자열로 변환해준다.
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

	char ipv4test[] = "147.46.114.110:8080";
	printf("IPv4 주소(변환전) = %s\n", ipv4test);
	sockaddr_in addr;
	int addrlen = sizeof(addr);

	if (WSAStringToAddressA(ipv4test, AF_INET, NULL, (sockaddr*)&addr, &addrlen) == SOCKET_ERROR)
	{
		err_display("WSAStringToAddressA()");
	}
	// WSAStringToAddress를 쓰면 주소가 네트워크 바이트로 변환되어 in_addr에 자동으로 넣어준다. 근데 가독성은 안좋아 보인다.
	printf("IPv4 주소(변환후) = %s port: %d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	
	char ipv4str[100];
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

int Exercise03_6(char** argv)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	const char* TEST = argv[1];
	hostent* ptr = gethostbyname(TEST);
	if (ptr == NULL) {
		err_display("gethostbyname()");
	}
	if (ptr->h_addrtype != AF_INET)
		return 0;
	printf("공식 도메인 이름: %s\n", ptr->h_name);
	for (int i = 0; ptr->h_aliases[i] != NULL; i++) {
		printf("별칭 도메인 이름[%d]: %s\n", i + 1, ptr->h_aliases[i]);
	}
	for (int i = 0; ptr->h_addr_list[i] != NULL; i++) {
		in_addr* ipv4addr = (in_addr*)ptr->h_addr_list[i];
		char* ipv4str = inet_ntoa(*ipv4addr);
		printf("IPv4 주소[%d] : %s\n",i+1, ipv4str);
	}

	WSACleanup();
	return 0;
}
int Exercise03_7(char** argv)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	char* ipv4Str = argv[1];
	sockaddr_in sock;
	int ipv4len = sizeof(sock);
	if (WSAStringToAddressA((LPSTR)ipv4Str, AF_INET, NULL, (sockaddr*)&sock, &ipv4len) == SOCKET_ERROR) {
		err_display("WSAStringToAddressA()");
	}
	hostent* ptr = gethostbyaddr((const char*)&sock.sin_addr, ipv4len, AF_INET);
	if (ptr == NULL) {
		err_display("gethostbyaddr()");
	}
	if (ptr->h_addrtype != AF_INET)
		return 0;
	printf("공식 도메인 이름: %s\n", ptr->h_name);
	for (int i = 0; ptr->h_aliases[i] != NULL; i++) {
		printf("별칭 도메인 이름[%d]: %s\n", i + 1, ptr->h_aliases[i]);
	}
	for (int i = 0; ptr->h_addr_list[i] != NULL; i++) {
		in_addr* ipv4addr = (in_addr*)ptr->h_addr_list[i];
		char* ipv4str = inet_ntoa(*ipv4addr);
		printf("IPv4 주소[%d] : %s", i + 1, ipv4str);
	}

	WSACleanup();
	return 0;
}

namespace Exercise03
{

	bool GetIpInfo(const char* name, addrinfo** result) // 메모리 해제 책임이 호출자(사용자)에게 있음. 좋은 구조는 아니다.
	{
		addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_flags = AI_CANONNAME;
		// gethostbyname = 프로토콜 종속적
		// getaddrinfo = 프로토콜 독립적  독립적이란건 ipv4, ipv6 알아서 변환해준다는 뜻이다
		// hints = NULL은 AF_UNSPEC, 다른 멤버 모두 0 (IPv4, IPv6만 처리하고 나머지는 상관안함)
		int errCode = getaddrinfo(name, NULL, &hints, result);
		if (errCode != 0) {
			err_display(errCode);
			return false;
		}
		return true;
	}

	bool GetDomainInfo(char* ipStr)
	{
		sockaddr_in saGNI;
		saGNI.sin_family = AF_INET;
		saGNI.sin_addr.s_addr = inet_addr(ipStr);

		char hostname[NI_MAXHOST];
		char servInfo[NI_MAXSERV];

		if (saGNI.sin_addr.s_addr == INADDR_NONE || saGNI.sin_addr.s_addr == INADDR_ANY) {
			printf("inet_addr returned INADDR_NONE or INADDR_ANY\n");
			WSACleanup();
			return 1;
		}

		int errCode = getnameinfo((sockaddr*)&saGNI, sizeof(sockaddr), hostname, NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV);
		if (errCode != 0) {
			err_display(errCode);
			return false;
		}

		printf("호스트 이름 = %s\n", hostname);

		return true;
	}
}

int Exercise03_8(char** argv)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	char* domain_name = argv[1];
	printf("입력받은 도메인 : %s\n", domain_name);
	
	addrinfo* result;
	if (Exercise03::GetIpInfo(domain_name, &result)) {
		for (addrinfo* ptr = result; ptr != NULL; ptr = ptr->ai_next) {
			if (ptr->ai_canonname != NULL)
				printf("공식 도메인 이름 : %s\n", ptr->ai_canonname);
			switch (ptr->ai_family) {
			case AF_UNSPEC: {
				printf("Unspecified\n");
				break;
			}
			case AF_INET: {
				sockaddr_in* ipv4 = (sockaddr_in*)ptr->ai_addr;
				printf("주소(IPv4): %s\n", inet_ntoa(ipv4->sin_addr));
				break;
			}
			case AF_INET6:
			{
				sockaddr* in6 = (sockaddr*)ptr->ai_addr;
				char buffer[46];
				DWORD buffer_len = 46;
				if (WSAAddressToStringA(in6, (DWORD)ptr->ai_addrlen, NULL, buffer, &buffer_len) != 0) {
					err_display("WSAAddressToStringA()");
				}

				printf("주소(IPv6): %s\n", buffer);
				break;
			}
			}
		}
		freeaddrinfo(result);
	}
	
	WSACleanup();
	return 0;
}

int Exercise03_9(char** argv)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	char* ip_name = argv[1];
	printf("입력받은 IP : %s\n", ip_name);
	
	Exercise03::GetDomainInfo(ip_name);


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


namespace Example03
{
#define TESTNAME "www.yahoo.com"

	bool GetIpAddr(const char* name, struct in_addr* addr)
	{
		struct hostent* ptr = gethostbyname(name);
		if (ptr == NULL) {
			err_display("gethostbyname()");
			return false;
		}

		if (ptr->h_addrtype != AF_INET)
			return false;

		printf("공식 도메인 이름: %s\n", ptr->h_name);
		for (int i = 0; ptr->h_aliases[i] != NULL; i++) {
			printf("별칭 도메인 이름[%d] : %s\n", i + 1, ptr->h_aliases[i]);
		}
		memcpy(addr, ptr->h_addr, ptr->h_length);

		for (int i = 0; ptr->h_addr_list[i] != NULL; ++i) {
			memcpy(addr, &ptr->h_addr[i], ptr->h_length);
			printf("IP 주소[%d] : %d.%d.%d.%d\n", i + 1, addr->S_un.S_un_b.s_b1, addr->S_un.S_un_b.s_b2, addr->S_un.S_un_b.s_b3, addr->S_un.S_un_b.s_b4);
		}
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
}

int Example03_3()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	printf("도메인 이름(변환 전) = %s\n", TESTNAME);

	// 도메인 이름->IP 주소
	struct in_addr addr;
	if (Example03::GetIpAddr(TESTNAME, &addr)) {
		char str[16];
		inet_ntop(AF_INET, &addr, str, sizeof(str));
		printf("IP 주소(변환 후) = %s\n", str);

		// IP 주소 -> 도메인 출력
		char name[256];
		if (Example03::GetDomainName(addr, name, sizeof(name))) {
			printf("도메인 이름(다시 변환 후) = %s\n", name);
		}
	}
	WSACleanup();
	return 0;
}

int Example04_01_s()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	SOCKET server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock == INVALID_SOCKET)
		err_quit("socket()");


	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(server_sock, (sockaddr*)&addr, sizeof(addr));
	
	listen(server_sock, SOMAXCONN);
	SOCKET client_sock;
	sockaddr_in client_addr;
	int addrlen = sizeof(client_addr);
	printf("연결 대기중..\n");
	client_sock = accept(server_sock, (sockaddr*)&client_addr, &addrlen);
	
	printf("클라이언트 [%s] 접속", inet_ntoa(client_addr.sin_addr));
	char buf[1024];
	while (recv(client_sock, buf, sizeof(buf), NULL) > 0) {
		printf("%s", buf);
		send(client_sock, buf, sizeof(buf), NULL);
	}
	closesocket(client_sock);
	closesocket(server_sock);
	WSACleanup();
	return 0;
}

int Example04_01_c()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	SOCKET client_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (client_sock == INVALID_SOCKET)
		err_quit("socket()");

	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.S_un.S_addr = 0x7F'00'00'01;
	printf("연결 시도..\n");
	connect(client_sock, (sockaddr*)&addr, sizeof(addr));

	printf("서버 접속 성공\n");
	char buf[1024];
	while (scanf("%s", buf) == 1) {
		send(client_sock, buf, sizeof(buf), NULL);

		recv(client_sock, buf, sizeof(buf), NULL);
		printf("recv: %s\n", buf);
	}
	closesocket(client_sock);
	WSACleanup();
	return 0;
}