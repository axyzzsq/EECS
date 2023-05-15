
#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

class Father {
private:
	int money;
public:
	void it_skill(void)
	{
		cout<<"father's it skill"<<endl;
	}

	int getMoney(void)
	{
		return money;
	}

	void setMoney(int money)
	{
		this->money = money;
	}
};

class Son : public Father {
private:
	int toy;
public:
	void play_game(void)
	{
		int m;
		
		cout<<"son paly game"<<endl;

		/* money -= 1; 
		 * ��: ����ֱ���ø��׵�˽��Ǯ
		 */

		/*
		 * ���ǿ�������Ҫ
		 */
		m = getMoney();
		m--;
		setMoney(m);
	}
};


int main(int argc, char **argv)
{
	Son s;

	s.setMoney(10);
	cout << s.getMoney()<<endl;

	s.it_skill();
	s.play_game();
	
	return 0;
}


