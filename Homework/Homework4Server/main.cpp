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

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 0;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("listen: socket()");

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9000);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(listen_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
		err_quit("bind()");
	if (listen(listen_sock, SOMAXCONN) == SOCKET_ERROR)
		err_quit("listen()");

	char buffer[BUF_SIZE];
	struct sockaddr_in client_addr;
	int addrlen;

	while (true) {
		addrlen = sizeof(client_addr);

		SOCKET client_sock = accept(listen_sock, (struct sockaddr*)&client_addr, &addrlen);
		if (client_sock == INVALID_SOCKET)
			err_quit("accept()");
		// 파일 이름 받기
		size_t recv_size;
		int ret_val;
		ret_val = recv(client_sock, (char*)&recv_size, sizeof(size_t), MSG_WAITALL);
		if (ret_val == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		if (ret_val == 0) break;

		char file_name[128];
		ret_val = recv(client_sock, file_name, recv_size, MSG_WAITALL);
		if (ret_val == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		if (ret_val == 0) break;
		file_name[recv_size] = '\0';

		// 파일 받기
		ret_val = recv(client_sock, (char*)&recv_size, sizeof(size_t), MSG_WAITALL);
		if (ret_val == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		if (ret_val == 0) break;

		size_t cur_size = 0;
		int recv_percent = 0;
		FILE* fp = fopen(file_name, "wb");
		while (cur_size + BUF_SIZE < recv_size) {
			ret_val = recv(client_sock, buffer, BUF_SIZE, MSG_WAITALL);
			if (ret_val == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			if (ret_val == 0) break;

			fwrite(buffer, sizeof(char), ret_val, fp);
			cur_size += ret_val;

			if (recv_percent != (double)cur_size / recv_size * 100) {
				recv_percent = (double)cur_size / recv_size * 100;
				printf("[ %s ] 전송률:%d%%\r", file_name, recv_percent);
			}
		}
		ret_val = recv(client_sock, buffer, recv_size - cur_size, MSG_WAITALL);
		if (ret_val == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		if (ret_val == 0) break;
		fwrite(buffer, sizeof(char), ret_val, fp);
		cur_size += ret_val;
		recv_percent = (double)cur_size / recv_size * 100;
		printf("[ %s ] 전송률:%d%%\r\n", file_name, recv_percent);


		fclose(fp);
		closesocket(client_sock);
	}


	closesocket(listen_sock);
	WSACleanup();
	return 0;
}