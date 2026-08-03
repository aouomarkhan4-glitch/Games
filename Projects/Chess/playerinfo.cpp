#pragma once
#include "playerinfo.hpp"

playerinfo::playerinfo()
{
	player = nullptr;
}
playerinfo::~playerinfo()
{
	if (player != nullptr)
	{
		delete[]player;
	}
}
unsigned int info::count = 1;
info::info() :playerinfo(), id(count)
{
	player = new char[8];
	strcpy_s(player, 8, "unknown");
	++count;

	for (int i = 0; i < 8; ++i)
	{
		pieces[i] = 'P';
	}
	pieces[8] = 'K';
	pieces[9] = 'Q';
	pieces[10] = 'B';
	pieces[11] = 'B';
	pieces[12] = 'N';
	pieces[13] = 'N';
	pieces[14] = 'R';
	pieces[15] = 'R';

}

void info::setinfo()
{
	char name[50];
	cout << " enter player " << id << " name -> ";
	cin.getline(name, 50);

	delete[]player;
	player = new char[strlen(name) + 1];
	strcpy_s(player, strlen(name) + 1, name);

}
//void info::turn(board& other)
//{
//	
//	other.move();
//}
void info::display()
{
	cout << " player  ( " << id << " ) -> " << player << endl;
}

info& info::obj()
{
	return *this;
}

info::~info()
{
	--count;

}
