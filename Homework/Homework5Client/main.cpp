#include "..\Common.h"
#include <commctrl.h>
#include <commdlg.h>
#pragma comment(lib, "comctl32.lib")

#include "resource.h"

#define SERVER_IP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    65536
#define PATH_SIZE 128


// 대화상자 프로시저
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
// 에디트 컨트롤 출력 함수
void DisplayText(const char* fmt, ...);
// 소켓 함수 오류 출력
void DisplayError(const char* msg);
// 소켓 통신 스레드 함수
DWORD WINAPI ClientMain(LPVOID arg);

SOCKET sock; // 소켓
char buf[BUFSIZE + 1]; // 데이터 송수신 버퍼
HWND hEdit;
HWND hFileButton;
HWND hProgress;
HANDLE hSendEvent;
char filePath[PATH_SIZE] = { 0 };

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 통신 스레드 생성
	CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

	hSendEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	// 대화상자 생성
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

	CloseHandle(hSendEvent);
	// 윈속 종료
	WSACleanup();

	return 0;
}

// 대화상자 프로시저
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        hEdit = GetDlgItem(hDlg, IDC_EDIT1);
        hFileButton = GetDlgItem(hDlg, IDC_BUTTON1);
		hProgress = GetDlgItem(hDlg, IDC_PROGRESS1);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
		case IDC_BUTTON1:
			OPENFILENAMEA fn;
			memset(&fn, 0, sizeof(fn));
			fn.lStructSize = sizeof(fn);
			fn.hwndOwner = hDlg;
			fn.lpstrFile = filePath;
			fn.nMaxFile = PATH_SIZE;
			fn.lpstrFilter = "All Files\0*.*\0Text Files\0*.txt\0";
			fn.nFilterIndex = 1;
			fn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetOpenFileNameA(&fn) == TRUE) {
				DisplayText("%s", filePath);
			}
			break;
        case IDOK:
			SetEvent(hSendEvent); // 파일전송 event
			break;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL); // 대화상자 닫기
			closesocket(sock); // 소켓 닫기
			return TRUE;
        }
		break;
    case WM_CLOSE:
        // 닫기 버튼(X)을 눌렀을 때
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}

// 에디트 컨트롤 출력 함수
void DisplayText(const char* fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	char cbuf[128];
	vsprintf(cbuf, fmt, arg);
	va_end(arg);

	int nLength = SendMessage(hEdit, EM_LINELENGTH, 0, 0);
	SendMessage(hEdit, EM_SETSEL, 0, (LPARAM)nLength);
	SendMessageA(hEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
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

// TCP 클라이언트 시작 부분
DWORD WINAPI ClientMain(LPVOID arg)
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9000);
	inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

	if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) err_quit("connect()");

	while (true) {

		WaitForSingleObject(hSendEvent, INFINITE);

		if (strlen(filePath) == 0) {
			MessageBoxA(NULL, "파일을 선택하지 않았습니다.", "경고", MB_ICONERROR);
			closesocket(sock);
			return 0;
		}

		FILE* fp = fopen(filePath, "rb");
		char buffer[BUFSIZE];
		size_t file_size;
		//파일 이름 전송
		const char* file_name = strrchr(filePath, '\\') + 1;
		size_t file_len = strlen(file_name);
		if (send(sock, (char*)&file_len, sizeof(size_t), 0) == SOCKET_ERROR)
			err_quit("file_len: send()");
		if (send(sock, file_name, file_len, 0) == SOCKET_ERROR)
			err_quit("file_name: send()");


		size_t cur_size = 0;
		int percent = 0;

		//파일 전송
		_fseeki64(fp, 0, SEEK_END);
		file_size = _ftelli64(fp);
		_fseeki64(fp, 0, SEEK_SET);
		if (send(sock, (char*)&file_size, sizeof(size_t), 0) == SOCKET_ERROR)
			err_quit("file_size: send()");
		size_t read_size;
		while ((read_size = fread(buffer, sizeof(char), BUFSIZE, fp)) > 0) {
			if (send(sock, buffer, read_size, 0) == SOCKET_ERROR)
				err_quit("file: send()");

			cur_size += read_size;
			if (percent != (int)((double)cur_size / file_size * 100)) {
				percent = (int)((double)cur_size / file_size * 100);
				SendMessage(hProgress, PBM_SETPOS, (WPARAM)percent, 0);
			}
		}
		MessageBoxA(NULL, "전송이 완료되었습니다.", "완료", MB_ICONERROR);
		fclose(fp);
	}
	closesocket(sock);
}
