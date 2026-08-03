#pragma once
#include <iostream>
#include "playerinfo.hpp"

using namespace std;

class Pawn
{
private:
	bool swapvalid;
public:
	Pawn();
	bool swap1bool();
	void swap1pwan(char arr[8][8], int fx, int fy, info& other);

	bool pawn1legal(int ix, int iy, int fx, int fy, const char arr[8][8], char tpieces[6], char Tpieces[6], info& other, int enpassantrow, int enpassantcol);
	bool pawn2legal(int ix, int iy, int fx, int fy, const char arr[8][8], char tpieces[6], char Tpieces[6], info& other, int enpassantrow, int enpassantcol);

	~Pawn();
};
