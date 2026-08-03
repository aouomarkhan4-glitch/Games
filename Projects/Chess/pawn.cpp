#include "Pawn.h"

Pawn::Pawn()
{
	swapvalid = false;
}

Pawn::~Pawn() {}

bool Pawn::swap1bool()
{
	return swapvalid;
}

bool Pawn::pawn1legal(int ix, int iy, int fx, int fy, const char arr[8][8], char tpieces[6], char Tpieces[6], info& other, int enpassantrow, int enpassantcol)
{
	swapvalid = false;
	bool valid = false;
	bool block = false;
	bool right = false;
	bool left = false;

	// block check
	if (ix < 7)
	{
		for (int i = 0; i < 6; ++i)
		{
			if ((arr[ix + 1][iy] == tpieces[i]) || (arr[ix + 1][iy] == Tpieces[i]))
			{
				block = true;
				break;
			}
		}
	}

	// right diagonal piece check
	if (iy < 7)
	{
		for (int a = 0; a < 6; ++a)
		{
			if (arr[ix + 1][iy + 1] == tpieces[a])
			{
				right = true;
				break;
			}
		}
	}

	// left diagonal piece check
	if (iy > 0)
	{
		for (int a = 0; a < 6; ++a)
		{
			if (arr[ix + 1][iy - 1] == tpieces[a])
			{
				left = true;
				break;
			}
		}
	}

	// initial double step
	if (ix == 1 && fy == iy && fx == ix + 2 && !block && arr[ix + 1][iy] == '-')
	{
		valid = true;
	}
	// one step forward
	else if (fx == ix + 1 && fy == iy && !block)
	{
		valid = true;
	}
	// capture right diagonal
	else if (fx == ix + 1 && fy == iy + 1 && right)
	{
		valid = true;
	}
	// capture left diagonal
	else if (fx == ix + 1 && fy == iy - 1 && left)
	{
		valid = true;
	}
	// en passant right
	else if (fx == ix + 1 && fy == iy + 1 && enpassantrow == ix && enpassantcol == iy + 1)
	{
		valid = true;
	}
	// en passant left
	else if (fx == ix + 1 && fy == iy - 1 && enpassantrow == ix && enpassantcol == iy - 1)
	{
		valid = true;
	}

	// swap check
	if (valid && fx == 7)
	{
		swapvalid = true;
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

void Pawn::swap1pwan(char arr[8][8], int fx, int fy, info& other)
{
	char choice;
	if (other.getid() == 1)
	{
		while (true)
		{
			cout << " YOU CAN NOW SWAP YOUR PAWN WITH :  " << endl;
			cout << " p -> " << endl;
			cout << " Q " << endl;
			cout << " B " << endl;
			cout << " N " << endl;
			cout << " R " << endl;
			cout << " enter your choice -> ";

			cin >> choice;
			if (choice == 'Q' || choice == 'B' || choice == 'R' || choice == 'N')
			{
				break;
			}
			else
			{
				cout << " invalid choice " << endl;
			}
		}
		arr[fx][fy] = choice;
	}
	else
	{
		if (other.getid() == 2)
		{
			while (true)
			{
				cout << " YOU CAN NOW SWAP YOUR PAWN WITH :  " << endl;
				cout << " p -> " << endl;
				cout << " q " << endl;
				cout << " b " << endl;
				cout << " n " << endl;
				cout << " r " << endl;
				cout << " enter your choice -> ";

				cin >> choice;
				if (choice == 'q' || choice == 'b' || choice == 'r' || choice == 'n')
				{
					break;
				}
				else
				{
					cout << " invalid choice " << endl;
				}
			}
			arr[fx][fy] = choice;
		}
	}
}

bool Pawn::pawn2legal(int ix, int iy, int fx, int fy, const char arr[8][8], char tpieces[6], char Tpieces[6], info& other, int enpassantrow, int enpassantcol)
{
	swapvalid = false;
	bool valid = false;
	bool block = false;
	bool right = false;
	bool left = false;

	// block check
	if (ix > 0)
	{
		for (int i = 0; i < 6; ++i)
		{
			if (arr[ix - 1][iy] == tpieces[i] || arr[ix - 1][iy] == Tpieces[i])
			{
				block = true;
				break;
			}
		}
	}

	// right diagonal piece check
	if (iy < 7)
	{
		for (int a = 0; a < 6; ++a)
		{
			if (arr[ix - 1][iy + 1] == Tpieces[a])
			{
				right = true;
				break;
			}
		}
	}

	// left diagonal piece check
	if (iy > 0)
	{
		for (int a = 0; a < 6; ++a)
		{
			if (arr[ix - 1][iy - 1] == Tpieces[a])
			{
				left = true;
				break;
			}
		}
	}

	// initial double step
	if (ix == 6 && fy == iy && fx == ix - 2 && !block && arr[ix - 1][iy] == '-')
	{
		valid = true;
	}
	// one step forward
	else if (fx == ix - 1 && fy == iy && !block)
	{
		valid = true;
	}
	// capture right diagonal
	else if (fx == ix - 1 && fy == iy + 1 && right)
	{
		valid = true;
	}
	// capture left diagonal
	else if (fx == ix - 1 && fy == iy - 1 && left)
	{
		valid = true;
	}
	// en passant right
	else if (fx == ix - 1 && fy == iy + 1 && enpassantrow == ix && enpassantcol == iy + 1)
	{
		valid = true;
	}
	// en passant left
	else if (fx == ix - 1 && fy == iy - 1 && enpassantrow == ix && enpassantcol == iy - 1)
	{
		valid = true;
	}

	// swap check
	if (valid && fx == 0)
	{
		swapvalid = true;
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
