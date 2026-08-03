#include "Rook.h"

Rook::Rook() {}
Rook::~Rook() {}

bool Rook::rock1legal(int ix, int iy, int fx, int fy, const char arr[8][8], char tpieces[6], char Tpieces[6], info& other)
{
	bool valid = true;
	bool rightblock = false;
	bool leftblock = false;
	bool upblock = false;
	bool downblock = false;

	if (fx == ix && (fy != iy)) // horizontal
	{
		if (fy > iy) // left to right
		{
			for (int i = iy + 1; i < fy; ++i)
			{
				for (int j = 0; j < 6; ++j)
				{
					if ((arr[ix][i] == tpieces[j]) || (arr[ix][i] == Tpieces[j]))
					{
						rightblock = true;
						valid = false;
						break;
					}
				}
				if (rightblock) break;
			}
		}
		else // right to left
		{
			for (int i = iy - 1; i > fy; --i)
			{
				for (int j = 0; j < 6; ++j)
				{
					if ((arr[ix][i] == tpieces[j]) || (arr[ix][i] == Tpieces[j]))
					{
						leftblock = true;
						valid = false;
						break;
					}
				}
				if (leftblock) break;
			}
		}
	}
	else if (fy == iy && fx != ix) // vertical
	{
		if (fx < ix) // bottom to up
		{
			for (int i = ix - 1; i > fx; --i)
			{
				for (int j = 0; j < 6; ++j)
				{
					if ((arr[i][fy] == tpieces[j]) || (arr[i][fy] == Tpieces[j]))
					{
						upblock = true;
						valid = false;
						break;
					}
				}
				if (upblock) break;
			}
		}
		else // up to bottom
		{
			for (int i = ix + 1; i < fx; ++i)
			{
				for (int j = 0; j < 6; ++j)
				{
					if ((arr[i][fy] == tpieces[j]) || (arr[i][fy] == Tpieces[j]))
					{
						downblock = true;
						valid = false;
						break;
					}
				}
				if (downblock) break;
			}
		}
	}
	else
	{
		valid = false;
	}

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
