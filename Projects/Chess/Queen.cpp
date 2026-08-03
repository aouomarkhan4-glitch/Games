#include "Queen.h"
#include <stdlib.h>

Queen::Queen() {}
Queen::~Queen() {}

bool Queen::queen1legal(int ix, int iy, int fx, int fy, const char arr[8][8], char tpieces[6], char Tpieces[6], info& other)
{
	Bishop bishop;
	Rook rook;

	// diagonal movement
	if (abs(fx - ix) == abs(fy - iy))
	{
		return bishop.bishop1legal(ix, iy, fx, fy, arr, tpieces, Tpieces, other);
	}

	// straight movement
	if ((fx == ix && fy != iy) || (fy == iy && fx != ix))
	{
		return rook.rock1legal(ix, iy, fx, fy, arr, tpieces, Tpieces, other);
	}

	return false;
}
