#include "Bishop.h"
#include <stdlib.h>

Bishop::Bishop() {}
Bishop::~Bishop() {}

bool Bishop::bishop1legal(int ix, int iy, int fx, int fy, const char arr[8][8], char tpieces[6], char Tpieces[6], info& other)
{
	bool valid = true;
	bool northeastblock = false;
	bool northwestblock = false;
	bool southeastblock = false;
	bool southwestblock = false;

	int rowdiff = abs(fx - ix); // when some point is in any point's diagonal then the rowdiff and coldiff
	int coldiff = abs(fy - iy); // between those points is same, so when they are same diagonal movement is legal

	if (rowdiff == coldiff)
	{
		if (fx < ix && fy > iy) // north-east direction
		{
			for (int i = 1; i < coldiff; ++i)
			{
				for (int j = 0; j < 6; ++j)
				{
					if (arr[ix - i][iy + i] == tpieces[j] || arr[ix - i][iy + i] == Tpieces[j])
					{
						northeastblock = true;
						valid = false;
					}
				}
			}
		}
		else if (fx < ix && fy < iy) // north-west direction
		{
			for (int i = 1; i < coldiff; ++i)
			{
				for (int j = 0; j < 6; ++j)
				{
					if (arr[ix - i][iy - i] == tpieces[j] || arr[ix - i][iy - i] == Tpieces[j])
					{
						northwestblock = true;
						valid = false;
					}
				}
			}
		}
		else if (fx > ix && fy > iy) // south-east direction
		{
			for (int i = 1; i < coldiff; ++i)
			{
				for (int j = 0; j < 6; ++j)
				{
					if (arr[ix + i][iy + i] == tpieces[j] || arr[ix + i][iy + i] == Tpieces[j])
					{
						southeastblock = true;
						valid = false;
					}
				}
			}
		}
		else if (fx > ix && fy < iy) // south-west direction
		{
			for (int i = 1; i < coldiff; ++i)
			{
				for (int j = 0; j < 6; ++j)
				{
					if (arr[ix + i][iy - i] == tpieces[j] || arr[ix + i][iy - i] == Tpieces[j])
					{
						southwestblock = true;
						valid = false;
					}
				}
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
