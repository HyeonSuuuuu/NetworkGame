#include "..\Common.h"


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
// ��� : CPU�� �޸𸮿��� �������� ���� ���� �� ���� ����� ����ؼ� �ø���..
	// htons�� �޸𸮰��� �ٷ��.. char*�� �޸𸮰��� �ٷ��.. ������ HIBYTE�� �������� ���� �ٷ��.
	// HIBYTE -> ��Ʈ����.. �׷��� �������� ���� �ε�� ������ ���� �����Ѵ�..
	// �׷��� �ش� �ڵ�� �׻� false�� �ȴ�.
}
bool IsLittileEndian03_01_v3()
{
	// �켱 ��Ʋ��������� üũ�ϴ°� 2byte�� ����ϴ�
	short i = 1;
	// htons�� �򿣵������ �ٲٴ°� �õ�
	if (htons(i)) { // network �ּ� ü�� ��ȯ �õ�
		return true; // ���������� ��Ʋ�����
	}
	else {
		return false; // ���������� �򿣵��(htons�Լ��� ���� �����ڰ� �򿣵���� �򿣵������ �ٲٴ°� ����ó�� �߱� ����)
	}

}
bool IsLittileEndian03_01_v4()
{
	union 
	{
		short s;
		char c[2];
	}u;
	// ���Ͽ��� �����ϰ� 1�� �ִ´�.
	u.s = 1;
	// ���Ͽ¿� 1���ְ� ù��° ����Ʈ�� 1���� Ȯ��
	if (u.c[0] == 1) {
		return true; // ���������� ��Ʋ�����
	}
	else {
		return false;
	}

}

void checkEndian03_1()
{
	if (IsLittileEndian03_01_v4()) {
		printf("�ý����� Littile-Endian ����� ����մϴ�.\n");
	}
	else {
		printf("�ý����� Big-Endian ����� ����մϴ�.\n");
	}
}

int main()
{
	printf("�ý����� ����Ʈ ���� ����� Ȯ���մϴ�.\n");
	checkEndian03_1();
	return 0;
}