#include "King.h"

King::King() {}
King::~King() {}

bool King::king1legal(int ix, int iy, int fx, int fy, const char arr[8][8], char tpieces[6], char Tpieces[6], info& other)
{
	bool valid = false;

	if (((fx == ix + 1) || (fx == ix - 1)) && fy == iy)
		valid = true;
	if (((fy == iy + 1) || (fy == iy - 1)) && fx == ix)
		valid = true;
	if (fx == ix - 1 && ((fy == iy + 1) || (fy == iy - 1)))
		valid = true;
	if (((fy == iy - 1) || (fy == iy + 1)) && fx == ix + 1)
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
				if (arr[fx][fy] == tpieces[h])
				{
					valid = false;
				}
			}
		}
	}
	return valid;
}
