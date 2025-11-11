#include "..\Common.h"

#define SERVERPORT 9000
#define BUFSIZE    65536

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// 에디트 컨트롤 출력 함수
void DisplayText(const char* fmt, ...);
// 소켓 함수 오류 출력
void DisplayError(const char* msg);
// 소켓 통신 스레드 함수
DWORD WINAPI ServerMain(LPVOID arg);
DWORD WINAPI ProcessClient(LPVOID arg);

HINSTANCE hInst; // 인스턴스 핸들
HWND hEdit; // 에디트 컨트롤
CRITICAL_SECTION cs; // 임계 영역
unsigned int client_id = 0;

struct THREAD_ARGS {
	SOCKET sock;
	unsigned int thread_id;
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	hInst = hInstance;
	InitializeCriticalSection(&cs);

	// 윈도우 클래스 등록
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = _T("MyWndClass");
	if (!RegisterClass(&wndclass)) return 1;

	// 윈도우 생성
	HWND hWnd = CreateWindow(_T("MyWndClass"), _T("TCP 서버"),
		WS_OVERLAPPEDWINDOW, 0, 0, 500, 220,
		NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) return 1;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 소켓 통신 스레드 생성
	CreateThread(NULL, 0, ServerMain, NULL, 0, NULL);

	// 메시지 루프
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DeleteCriticalSection(&cs);
	return (int)msg.wParam;
}

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
		hEdit = CreateWindow(_T("edit"), NULL,
			WS_CHILD | WS_VISIBLE | WS_HSCROLL |
			WS_VSCROLL | ES_AUTOHSCROLL |
			ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY,
			0, 0, 0, 0, hWnd, (HMENU)100, hInst, NULL);
		return 0;
	case WM_SIZE:
		MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;
	case WM_SETFOCUS:
		SetFocus(hEdit);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// 에디트 컨트롤 출력 함수
void DisplayText(const char* fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	char cbuf[128];
	vsprintf(cbuf, fmt, arg);
	va_end(arg);

	EnterCriticalSection(&cs);
	int nLength = GetWindowTextLength(hEdit);
	SendMessage(hEdit, EM_SETSEL, nLength, nLength);
	SendMessageA(hEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
	LeaveCriticalSection(&cs);
}

void DisplayLineText(const char* fmt, int thread_id, ...)
{
	va_list arg;
	va_start(arg, fmt);
	char cbuf[128];
	vsprintf(cbuf, fmt, arg);
	va_end(arg);


	EnterCriticalSection(&cs);
	int start = (int)SendMessageA(hEdit, EM_LINEINDEX, (WPARAM)thread_id, 0);
	if (start == -1) {
		LeaveCriticalSection(&cs);
		return;
	}

	int lineLength = SendMessage(hEdit, EM_LINELENGTH, start, 0);
	int end = start + lineLength;
	SendMessage(hEdit, EM_SETSEL, start, end);
	SendMessageA(hEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
	LeaveCriticalSection(&cs);
}

// 소켓 함수 오류 출력
void DisplayError(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	DisplayText("[%s] %s\r\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// TCP 서버 시작 부분
DWORD WINAPI ServerMain(LPVOID arg)
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			DisplayError("accept()");
			break;
		}

		DisplayText("\r\n\r\n"); // 접속마다 행추가

		// 스레드 생성
		THREAD_ARGS* args = new THREAD_ARGS{ client_sock, client_id++ };
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)args, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}

// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval;
	THREAD_ARGS* args = (THREAD_ARGS*)arg;
	SOCKET client_sock = (SOCKET)args->sock;
	unsigned int thread_id = args->thread_id;
	delete arg;

	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	// 파일 이름 받기
	size_t recv_size;
	int ret_val;
	ret_val = recv(client_sock, (char*)&recv_size, sizeof(size_t), MSG_WAITALL);
	if (ret_val == SOCKET_ERROR) {
		err_display("recv()");
		return -1;
	}
	if (ret_val == 0)
		return -1;

	char file_name[128];
	ret_val = recv(client_sock, file_name, recv_size, MSG_WAITALL);
	if (ret_val == SOCKET_ERROR) {
		err_display("recv()");
		return -1;
	}
	if (ret_val == 0)
		return -1;
	file_name[recv_size] = '\0';

	// 파일 받기
	ret_val = recv(client_sock, (char*)&recv_size, sizeof(size_t), MSG_WAITALL);
	if (ret_val == SOCKET_ERROR) {
		err_display("recv()");
		return -1;
	}
	if (ret_val == 0)
		return -1;

	size_t cur_size = 0;
	int recv_percent = 0;
	FILE* fp = fopen(file_name, "wb");

	while (cur_size + BUFSIZE < recv_size) {
		ret_val = recv(client_sock, buf, BUFSIZE, MSG_WAITALL);
		if (ret_val == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		if (ret_val == 0) break;

		fwrite(buf, sizeof(char), ret_val, fp);
		cur_size += ret_val;

		if (recv_percent != static_cast<int>(static_cast<double>(cur_size) / recv_size * 100)) {
			recv_percent = (double)cur_size / recv_size * 100;
			DisplayLineText("[%d: %s ] 전송률:%d%%", thread_id, thread_id, file_name, recv_percent);
		}
	}
	ret_val = recv(client_sock, buf, recv_size - cur_size, MSG_WAITALL);
	if (ret_val == SOCKET_ERROR) {
		err_display("recv()");
		return -1;
	}
	if (ret_val == 0)
		return -1;
	fwrite(buf, sizeof(char), ret_val, fp);
	cur_size += ret_val;
	recv_percent = (double)cur_size / recv_size * 100;
	DisplayLineText("[%d: %s ] 전송률:%d%%", thread_id, thread_id, file_name, recv_percent);

	fclose(fp);
	closesocket(client_sock);
	DisplayLineText("[%d: 수신완료", thread_id, thread_id);
	return 0;
}
