#include "..\Common.h"


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
// 결론 : CPU가 메모리에서 레지스터 값을 꺼낼 때 정렬 방식을 고려해서 올린다..
	// htons는 메모리값을 다루고.. char*도 메모리값을 다룬다.. 하지만 HIBYTE는 레지스터 값을 다룬다.
	// HIBYTE -> 비트연산.. 그러니 레지스터 값에 로드된 논리적인 값을 연산한다..
	// 그러니 해당 코드는 항상 false가 된다.
}
bool IsLittileEndian03_01_v3()
{
	// 우선 리틀엔디안인지 체크하는건 2byte면 충분하다
	short i = 1;
	// htons로 빅엔디안으로 바꾸는걸 시도
	if (htons(i)) { // network 주소 체계 변환 시도
		return true; // 성공했으면 리틀엔디안
	}
	else {
		return false; // 실패했으면 빅엔디안(htons함수를 만든 개발자가 빅엔디안을 빅엔디안으로 바꾸는걸 예외처리 했기 때문)
	}

}
bool IsLittileEndian03_01_v4()
{
	union 
	{
		short s;
		char c[2];
	}u;
	// 유니온을 선언하고 1을 넣는다.
	u.s = 1;
	// 유니온에 1을넣고 첫번째 바이트가 1인지 확인
	if (u.c[0] == 1) {
		return true; // 성공했으면 리틀엔디안
	}
	else {
		return false;
	}

}

void checkEndian03_1()
{
	if (IsLittileEndian03_01_v4()) {
		printf("시스템은 Littile-Endian 방식을 사용합니다.\n");
	}
	else {
		printf("시스템은 Big-Endian 방식을 사용합니다.\n");
	}
}

int main()
{
	printf("시스템의 바이트 정렬 방식을 확인합니다.\n");
	checkEndian03_1();
	return 0;
}