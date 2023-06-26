
#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

class Father {
private:
	int money;

protected:
	int room_key;
	
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
	//using Father::it_skill;
public:
	using Father::room_key; //调整访问权限，使得子类可以访问父类的protected成员
	//using Father::money;
	//子类能调整的是父类的protected成员，而不能调整父类的private成员
	//只能调它看得见的基类成员，不能调它看不见的基类成员
	void play_game(void)
	{
		int m;
		
		cout<<"son paly game"<<endl;

		/* money -= 1; 
		 * 错: 不能直接拿父亲的私房钱
		 */

		/*
		 * 但是可以问他要
		 */
		m = getMoney();
		m--;
		setMoney(m);

		room_key = 1; 
	}
};


int main(int argc, char **argv)
{
	Son s;

	s.setMoney(10);
	cout << s.getMoney()<<endl;

	s.it_skill();
	s.play_game();

	s.room_key = 1;
	
	return 0;
}


