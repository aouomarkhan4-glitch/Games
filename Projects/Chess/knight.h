#pragma once
#include <iostream>
#include "playerinfo.hpp"

using namespace std;

class Knight
{
public:
	Knight();
	bool knight1legal(int ix, int iy, int fx, int fy, const char arr[8][8], char tpieces[6], char Tpieces[6], info& other);
	~Knight();
};
