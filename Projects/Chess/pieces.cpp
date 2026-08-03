#include "pieces.h"

pieces::pieces() {}
pieces::~pieces() {}

bool pieces::isincheck(int kingx, int kingy, char arr[8][8], char tpieces[6], char Tpieces[6], info& current, info& enemy, int enpassantrow, int enpassantcol)
{
	Pawn pawn;
	Rook rook;
	Bishop bishop;
	Knight knight;
	Queen queen;
	King king;

	// check if any enemy piece can legally reach the king's position
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (current.getid() == 1) // player 1 king being checked, player 2 pieces attacking
			{
				for (int k = 0; k < 6; ++k)
				{
					if (arr[i][j] == tpieces[k]) // enemy piece found
					{
						if (tpieces[k] == 'p' && pawn.pawn2legal(i, j, kingx, kingy, arr, tpieces, Tpieces, enemy, enpassantrow, enpassantcol))
							return true;
						if (tpieces[k] == 'r' && rook.rock1legal(i, j, kingx, kingy, arr, tpieces, Tpieces, enemy))
							return true;
						if (tpieces[k] == 'b' && bishop.bishop1legal(i, j, kingx, kingy, arr, tpieces, Tpieces, enemy))
							return true;
						if (tpieces[k] == 'n' && knight.knight1legal(i, j, kingx, kingy, arr, tpieces, Tpieces, enemy))
							return true;
						if (tpieces[k] == 'q' && queen.queen1legal(i, j, kingx, kingy, arr, tpieces, Tpieces, enemy))
							return true;
						if (tpieces[k] == 'k' && king.king1legal(i, j, kingx, kingy, arr, tpieces, Tpieces, enemy))
							return true;
					}
				}
			}
			else // player 2 king being checked, player 1 pieces attacking
			{
				for (int k = 0; k < 6; ++k)
				{
					if (arr[i][j] == Tpieces[k])
					{
						if (Tpieces[k] == 'P' && pawn.pawn1legal(i, j, kingx, kingy, arr, tpieces, Tpieces, enemy, enpassantrow, enpassantcol))
							return true;
						if (Tpieces[k] == 'R' && rook.rock1legal(i, j, kingx, kingy, arr, tpieces, Tpieces, enemy))
							return true;
						if (Tpieces[k] == 'B' && bishop.bishop1legal(i, j, kingx, kingy, arr, tpieces, Tpieces, enemy))
							return true;
						if (Tpieces[k] == 'N' && knight.knight1legal(i, j, kingx, kingy, arr, tpieces, Tpieces, enemy))
							return true;
						if (Tpieces[k] == 'Q' && queen.queen1legal(i, j, kingx, kingy, arr, tpieces, Tpieces, enemy))
							return true;
						if (Tpieces[k] == 'K' && king.king1legal(i, j, kingx, kingy, arr, tpieces, Tpieces, enemy))
							return true;
					}
				}
			}
		}
	}
	return false;
}

bool pieces::checkmate(int kingx, int kingy, char arr[8][8], char tpieces[6], char Tpieces[6], info& current, info& enemy, int enpassantrow, int enpassantcol)
{
	if (!isincheck(kingx, kingy, arr, tpieces, Tpieces, current, enemy, enpassantrow, enpassantcol))
	{
		return false;
	}
	return !haslegalmove(arr, tpieces, Tpieces, current, enemy, enpassantrow, enpassantcol);
}

bool pieces::checklegal(int ix, int iy, int fx, int fy, char arr[8][8], char tpieces[6], char Tpieces[6], info& current, info& enemy, int enpassantrow, int enpassantcol)
{
	// save pieces
	char movedpiece = arr[ix][iy];
	char capturedpiece = arr[fx][fy];

	// make temporary move
	arr[fx][fy] = movedpiece;
	arr[ix][iy] = '-';

	// find current player's king
	char kingchar;
	if (current.getid() == 1)
		kingchar = 'K';
	else
		kingchar = 'k';

	int kingx = -1;
	int kingy = -1;

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (arr[i][j] == kingchar)
			{
				kingx = i;
				kingy = j;
			}
		}
	}

	// check if still in check after move
	bool stillincheck = isincheck(kingx, kingy, arr, tpieces, Tpieces, current, enemy, enpassantrow, enpassantcol);

	// undo move
	arr[ix][iy] = movedpiece;
	arr[fx][fy] = capturedpiece;

	// legal only if NOT still in check
	return !stillincheck;
}

bool pieces::haslegalmove(char arr[8][8], char tpieces[6], char Tpieces[6], info& current, info& enemy, int enpassantrow, int enpassantcol)
{
	Pawn pawn;
	Rook rook;
	Bishop bishop;
	Knight knight;
	Queen queen;
	King king;

	for (int ix = 0; ix < 8; ++ix)
	{
		for (int iy = 0; iy < 8; ++iy)
		{
			char piece = arr[ix][iy];
			bool friendly = false;

			if (current.getid() == 1)
			{
				for (int k = 0; k < 6; ++k)
				{
					if (piece == Tpieces[k])
					{
						friendly = true;
					}
				}
			}
			else
			{
				for (int k = 0; k < 6; ++k)
				{
					if (piece == tpieces[k])
					{
						friendly = true;
					}
				}
			}

			if (!friendly) continue;

			for (int fx = 0; fx < 8; ++fx)
			{
				for (int fy = 0; fy < 8; ++fy)
				{
					bool legal = false;

					if (current.getid() == 1)
					{
						if (piece == 'P') legal = pawn.pawn1legal(ix, iy, fx, fy, arr, tpieces, Tpieces, current, enpassantrow, enpassantcol);
						if (piece == 'R') legal = rook.rock1legal(ix, iy, fx, fy, arr, tpieces, Tpieces, current);
						if (piece == 'B') legal = bishop.bishop1legal(ix, iy, fx, fy, arr, tpieces, Tpieces, current);
						if (piece == 'N') legal = knight.knight1legal(ix, iy, fx, fy, arr, tpieces, Tpieces, current);
						if (piece == 'Q') legal = queen.queen1legal(ix, iy, fx, fy, arr, tpieces, Tpieces, current);
						if (piece == 'K') legal = king.king1legal(ix, iy, fx, fy, arr, tpieces, Tpieces, current);
					}
					else
					{
						if (piece == 'p') legal = pawn.pawn2legal(ix, iy, fx, fy, arr, tpieces, Tpieces, current, enpassantrow, enpassantcol);
						if (piece == 'r') legal = rook.rock1legal(ix, iy, fx, fy, arr, tpieces, Tpieces, current);
						if (piece == 'b') legal = bishop.bishop1legal(ix, iy, fx, fy, arr, tpieces, Tpieces, current);
						if (piece == 'n') legal = knight.knight1legal(ix, iy, fx, fy, arr, tpieces, Tpieces, current);
						if (piece == 'q') legal = queen.queen1legal(ix, iy, fx, fy, arr, tpieces, Tpieces, current);
						if (piece == 'k') legal = king.king1legal(ix, iy, fx, fy, arr, tpieces, Tpieces, current);
					}

					if (legal)
					{
						bool safe = checklegal(ix, iy, fx, fy, arr, tpieces, Tpieces, current, enemy, enpassantrow, enpassantcol);
						if (safe)
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool pieces::stalemate(int kingx, int kingy, char arr[8][8], char tpieces[6], char Tpieces[6], info& current, info& enemy, int enpassantrow, int enpassantcol)
{
	if (isincheck(kingx, kingy, arr, tpieces, Tpieces, current, enemy, enpassantrow, enpassantcol))
	{
		return false;
	}
	return !haslegalmove(arr, tpieces, Tpieces, current, enemy, enpassantrow, enpassantcol);
}
