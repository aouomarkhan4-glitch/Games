#pragma once
#include <iostream>
#include "playerinfo.hpp"
#include "Pawn.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Queen.h"
#include "King.h"

using namespace std;

class pieces
{
public:
	pieces();

	bool isincheck(int kingx, int kingy, char arr[8][8], char tpieces[6], char Tpieces[6], info& current, info& enemy, int enpassantrow, int enpassantcol);

	// current = player whose king is in check, enemy = player who is attacking (whose turn it is)
	bool checkmate(int kingx, int kingy, char arr[8][8], char tpieces[6], char Tpieces[6], info& current, info& enemy, int enpassantrow, int enpassantcol);

	// checks that after a move the king is not left in check (used to filter illegal moves)
	bool checklegal(int ix, int iy, int fx, int fy, char arr[8][8], char tpieces[6], char Tpieces[6], info& current, info& enemy, int enpassantrow, int enpassantcol);

	// searches all friendly pieces for any legal move that leaves king safe; used for both stalemate and checkmate
	bool haslegalmove(char arr[8][8], char tpieces[6], char Tpieces[6], info& current, info& enemy, int enpassantrow, int enpassantcol);

	// when player has no legal move and king is not in check -> draw
	bool stalemate(int kingx, int kingy, char arr[8][8], char tpieces[6], char Tpieces[6], info& current, info& enemy, int enpassantrow, int enpassantcol);

	~pieces();
};
