#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <ws2tcpip.h>

#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "ws2_32")

void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
}

void err_display(int errCode)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, "오류", MB_ICONERROR);
	LocalFree(lpMsgBuf);
}




#define BUF_SIZE 65536

int main(int argc, char** argv)
{
	if (argc != 3)
		return 0;
	const char* server_ip = argv[1];
	const char* file_path = argv[2];


	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 0;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9000);
	inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

	if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) err_quit("connect()");

	FILE* fp = fopen(file_path, "rb");
	char buffer[BUF_SIZE];
	size_t file_size;
	//파일 이름 전송
	const char* file_name = strrchr(file_path, '\\') + 1;
	size_t file_len = strlen(file_name);
	if (send(sock, (char*)&file_len, sizeof(size_t), 0) == SOCKET_ERROR)
		err_quit("file_len: send()");
	if (send(sock, file_name, file_len, 0) == SOCKET_ERROR)
		err_quit("file_name: send()");

	//파일 전송
	_fseeki64(fp, 0, SEEK_END);
	file_size = _ftelli64(fp);
	_fseeki64(fp, 0, SEEK_SET);
	if (send(sock, (char*)&file_size, sizeof(size_t), 0) == SOCKET_ERROR)
		err_quit("file_size: send()");
	size_t read_size;
	while ((read_size = fread(buffer, sizeof(char), BUF_SIZE, fp)) > 0) {
		if (send(sock, buffer, read_size, 0) == SOCKET_ERROR)
			err_quit("file: send()");
	}

	fclose(fp);
	closesocket(sock);
	WSACleanup();
	return 0;
}