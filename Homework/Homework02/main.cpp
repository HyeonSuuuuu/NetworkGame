#include "..\Common.h"


bool IsLittileEndian()
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

void checkEndian()
{
	if (IsLittileEndian()) {
		printf("�ý����� Littile-Endian ����� ����մϴ�.\n");
	}
	else {
		printf("�ý����� Big-Endian ����� ����մϴ�.\n");
	}
}



int main()
{
	printf("�ý����� ����Ʈ ���� ����� Ȯ���մϴ�.\n");
	checkEndian();
	return 0;
}
