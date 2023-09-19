#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#define MAX_TEXT 512
//��Ϣ�ṹ��
struct my_msg_st{
	long int my_msg_type;       				//��Ϣ����
	char anytext[MAX_TEXT];     				//��Ϣ����
};
int main()
{
	int idx = 1;
	int msgid;
	struct my_msg_st data;
	char buf[BUFSIZ];						//���û������
	msgid = msgget((key_t)1000, 0664 | IPC_CREAT);//������Ϣ����
	if (msgid == -1){
		perror("msgget err");
		exit(-1);
	}
	while (idx < 5){							//������Ϣ
		printf("enter some text:");
		fgets(buf, BUFSIZ, stdin);
		data.my_msg_type = rand() % 3 + 1;        	//�����ȡ��Ϣ����
		strcpy(data.anytext, buf);
		//������Ϣ
		if (msgsnd(msgid, (void*)&data, sizeof(data), 0) == -1){
			perror("msgsnd err");
			exit(-1);
		}
		idx++;
	}
	return 0;
}
