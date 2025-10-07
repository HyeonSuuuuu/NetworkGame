#include "..\Common.h"


bool IsLittileEndian()
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

void checkEndian()
{
	if (IsLittileEndian()) {
		printf("시스템은 Littile-Endian 방식을 사용합니다.\n");
	}
	else {
		printf("시스템은 Big-Endian 방식을 사용합니다.\n");
	}
}



int main()
{
	printf("시스템의 바이트 정렬 방식을 확인합니다.\n");
	checkEndian();
	return 0;
}
