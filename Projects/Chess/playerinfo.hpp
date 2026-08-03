#pragma once
#include<iostream>
//#include "board.hpp"


using namespace std;
class info;
class playerinfo
{

protected:
	char* player;

public:
	playerinfo();

	virtual void setinfo() = 0;
	virtual void display() = 0;
	/* virtual void turn(board& other) = 0;*/
	virtual ~playerinfo();
	virtual info& obj() = 0;
	virtual int getid() = 0;
};



class info :public playerinfo
{
protected:
	static unsigned int count;
	const int id;
	char pieces[16];


public:
	info();
	void setinfo()override;
	void display();
	info& obj();
	int getid()
	{
		return id;
	}
	//void turn(board&other);

	~info();
};