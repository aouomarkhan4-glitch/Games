#pragma once
#include <iostream>
#include "playerinfo.hpp"
#include "Rook.h"
#include "Bishop.h"

using namespace std;

class Queen
{
public:
	Queen();
	bool queen1legal(int ix, int iy, int fx, int fy, const char arr[8][8], char tpieces[6], char Tpieces[6], info& other);
	~Queen();
};
