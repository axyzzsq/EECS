
#include <stdio.h>

int main(int argc, char **argv)
{
	double d = 100.1;
	int i = d;  // doubleתΪint
	//const��ʾֻ���������޸ģ����ǿ���ǿ��ת��Ϊ��const��
	//�������û�м�const���������ᱨwarning����Ϊconst char *תΪ�ɶ���д��char *�ǲ���ȫ�ġ�
	const char *str = "100ask.taobao.com";
	
	//����const_cast��ʾǿ��ת��Ϊ��const
	char *str2 = const_cast<char *>(str);

	//reinterpret_cast<type-id>��ʾ��һ��ָ��ת��Ϊ��һ��ָ������
	int *p = reinterpret_cast<int *>(str2); // char *תΪint * 

	printf("i = %d, str = 0x%x, p = 0x%x\n", i, reinterpret_cast<unsigned int>(str), reinterpret_cast<unsigned int>(p));

	return 0;
}

