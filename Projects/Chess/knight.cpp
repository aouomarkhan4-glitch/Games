#include "Knight.h"

Knight::Knight() {}
Knight::~Knight() {}

bool Knight::knight1legal(int ix, int iy, int fx, int fy, const char arr[8][8], char tpieces[6], char Tpieces[6], info& other)
{
	bool valid = false;

	// up up (right/left) / down down (right/left)
	if (fx == ix - 2 && fy == iy + 1 && ix >= 2 && iy <= 6) // up up right
		valid = true;
	if (fx == ix - 2 && fy == iy - 1 && ix >= 2 && iy >= 1) // up up left
		valid = true;
	if (fx == ix + 2 && fy == iy + 1 && ix <= 5 && iy <= 6) // down down right
		valid = true;
	if (fx == ix + 2 && fy == iy - 1 && ix <= 5 && iy >= 1) // down down left
		valid = true;

	// right right (down/up) / left left (down/up)
	if (fx == ix - 1 && fy == iy + 2 && ix >= 1 && iy <= 5) // right right up
		valid = true;
	if (fx == ix + 1 && fy == iy + 2 && ix <= 6 && iy <= 5) // right right down
		valid = true;
	if (fx == ix - 1 && fy == iy - 2 && ix >= 1 && iy >= 2) // left left up
		valid = true;
	if (fx == ix + 1 && fy == iy - 2 && ix <= 6 && iy >= 2) // left left down
		valid = true;

	if (valid)
	{
		if (other.getid() == 1)
		{
			for (int h = 0; h < 6; ++h)
			{
				if (arr[fx][fy] == Tpieces[h])
				{
					valid = false;
				}
			}
		}
		if (other.getid() == 2)
		{
			for (int h = 0; h < 6; ++h)
			{
				if (tpieces[h] == arr[fx][fy])
				{
					valid = false;
				}
			}
		}
	}
	return valid;
}
