#pragma once
#include <iostream>
#include "playerinfo.hpp"

using namespace std;

class Rook
{
public:
	Rook();
	bool rock1legal(int ix, int iy, int fx, int fy, const char arr[8][8], char tpieces[6], char Tpieces[6], info& other);
	~Rook();
};
