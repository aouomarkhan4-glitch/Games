#include "board.h"

board::board() : x(8), y(8)
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			arr[i][j] = '-';
		}
	}
	srows    = "87654321";
	scolumns = "abcdefgh";
	tpieces[0] = 'p';
	tpieces[1] = 'k';
	tpieces[2] = 'q';
	tpieces[3] = 'b';
	tpieces[4] = 'n';
	tpieces[5] = 'r';

	Tpieces[0] = 'P';
	Tpieces[1] = 'K';
	Tpieces[2] = 'Q';
	Tpieces[3] = 'B';
	Tpieces[4] = 'N';
	Tpieces[5] = 'R';

	enpassantrow = -1;
	enpassantcol = -1;
	pendingPromotionRow = -1;
	pendingPromotionCol = -1;
}

void board::createboard()
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			arr[i][j] = '-';
		}
	}
}

void board::display()
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			cout << setw(4) << " ";
		}
		cout << setw(4) << srows[i] << " ";
		cout << "";
		for (int j = 0; j < 8; ++j)
		{
			cout << setw(4) << arr[i][j];
		}
		cout << "";
		cout << endl;
	}
}

void board::center()
{
	for (int i = 0; i < 7; ++i)
	{
		cout << endl;
	}
	for (int j = 0; j < 11; ++j)
	{
		cout << setw(4) << " ";
	}
	for (int k = 0; k < 8; ++k)
	{
		cout << setw(4) << scolumns[k];
	}
	cout << endl;
	cout << endl;

	display();
}

void board::allotpices()
{
	for (int j = 0; j < 8; ++j)
	{
		arr[1][j] = Tpieces[0];
		arr[6][j] = tpieces[0];
	}
	arr[0][0] = Tpieces[5];
	arr[0][7] = Tpieces[5];
	arr[7][0] = tpieces[5];
	arr[7][7] = tpieces[5];

	arr[0][1] = Tpieces[4];
	arr[0][6] = Tpieces[4];
	arr[7][1] = tpieces[4];
	arr[7][6] = tpieces[4];

	arr[0][2] = Tpieces[3];
	arr[0][5] = Tpieces[3];
	arr[7][2] = tpieces[3];
	arr[7][5] = tpieces[3];

	arr[0][3] = Tpieces[2];
	arr[0][4] = Tpieces[1];
	arr[7][3] = tpieces[2];
	arr[7][4] = tpieces[1];
}

bool board::move(info& other, info& enemy)
{
	while (true)
	{
		system("pause");
		system("cls");
		other.display();

		center();

		string iposition;
		string fposition;
		int tix;
		int tiy;
		int tfx;
		int tfy;

		while (true)
		{
			cout << " select your piece (7a/a7) -> ";
			cin >> iposition;

			cout << " enter  your move  (6a/a6) -> ";
			cin >> fposition;

			// for initial -> 7a / final -> 6a
			if (iposition[0] >= '1' && iposition[0] <= '8' && fposition[0] >= '1' && fposition[0] <= '8' && iposition[1] >= 'a' && iposition[1] <= 'h' && fposition[1] >= 'a' && fposition[1] <= 'h')
			{
				for (int i = 0; i < 8; ++i)
				{
					if (iposition[0] == srows[i])    tix = i;
					if (iposition[1] == scolumns[i]) tiy = i;
					if (fposition[0] == srows[i])    tfx = i;
					if (fposition[1] == scolumns[i]) tfy = i;
				}
				break;
			}
			else if (iposition[0] >= '1' && iposition[0] <= '8' && fposition[1] >= '1' && fposition[1] <= '8' && iposition[1] >= 'a' && iposition[1] <= 'h' && fposition[0] >= 'a' && fposition[0] <= 'h')
			{
				for (int i = 0; i < 8; ++i)
				{
					if (iposition[0] == srows[i])    tix = i;
					if (iposition[1] == scolumns[i]) tiy = i;
					if (fposition[0] == scolumns[i]) tfy = i;
					if (fposition[1] == srows[i])    tfx = i;
				}
				break;
			}
			else if (iposition[1] >= '1' && iposition[1] <= '8' && fposition[0] >= '1' && fposition[0] <= '8' && iposition[0] >= 'a' && iposition[0] <= 'h' && fposition[1] >= 'a' && fposition[1] <= 'h')
			{
				for (int i = 0; i < 8; ++i)
				{
					if (iposition[0] == scolumns[i]) tiy = i;
					if (iposition[1] == srows[i])    tix = i;
					if (fposition[0] == srows[i])    tfx = i;
					if (fposition[1] == scolumns[i]) tfy = i;
				}
				break;
			}
			else if (iposition[1] >= '1' && iposition[1] <= '8' && fposition[1] >= '1' && fposition[1] <= '8' && iposition[0] >= 'a' && iposition[0] <= 'h' && fposition[0] >= 'a' && fposition[0] <= 'h')
			{
				for (int i = 0; i < 8; ++i)
				{
					if (iposition[1] == srows[i])    tix = i;
					if (iposition[0] == scolumns[i]) tiy = i;
					if (fposition[1] == srows[i])    tfx = i;
					if (fposition[0] == scolumns[i]) tfy = i;
				}
				break;
			}
			else
			{
				cout << " invalid input " << endl;
			}
		}

		// ── PLAYER 1 TURN ──
		if (other.getid() == 1)
		{
			bool found = false;
			for (int i = 0; i < 6; ++i)
			{
				if (Tpieces[i] == arr[tix][tiy])
				{
					found = true;
				}
			}
			if (found)
			{
				bool legal = false;

				if (arr[tix][tiy] == 'P')
					legal = pawnmove.pawn1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other, enpassantrow, enpassantcol);
				if (arr[tix][tiy] == 'R')
					legal = rookmove.rock1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);
				if (arr[tix][tiy] == 'B')
					legal = bishopmove.bishop1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);
				if (arr[tix][tiy] == 'N')
					legal = knightmove.knight1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);
				if (arr[tix][tiy] == 'Q')
					legal = queenmove.queen1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);
				if (arr[tix][tiy] == 'K')
					legal = kingmove.king1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);

				if (legal)
					legal = legalmove.checklegal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other, enemy, enpassantrow, enpassantcol);

				if (legal)
				{
					arr[tfx][tfy] = arr[tix][tiy];
					arr[tix][tiy] = '-';

					// reset en passant every move
					enpassantrow = -1;
					enpassantcol = -1;

					// if pawn double stepped record position
					if (arr[tfx][tfy] == 'P' && tfx == tix + 2)
					{
						enpassantrow = tfx;
						enpassantcol = tfy;
					}

					// if en passant capture happened remove the captured pawn
					if (arr[tfx][tfy] == 'P' && tfx == tix + 1 && tfy != tiy && arr[tix][tfy] == 'p')
					{
						arr[tix][tfy] = '-';
					}

					bool swaplegal = pawnmove.swap1bool();
					if (swaplegal && arr[tfx][tfy] == 'P')
					{
						pawnmove.swap1pwan(arr, tfx, tfy, other);
					}

					// find enemy king position
					char enemyking = 'k';
					int kingx = -1, kingy = -1;

					for (int i = 0; i < 8; ++i)
					{
						for (int j = 0; j < 8; ++j)
						{
							if (arr[i][j] == enemyking)
							{
								kingx = i;
								kingy = j;
							}
						}
					}

					if (kingx != -1)
					{
						if (legalmove.checkmate(kingx, kingy, arr, tpieces, Tpieces, enemy, other, enpassantrow, enpassantcol))
						{
							system("cls");
							other.display();
							center();
							cout << " CHECKMATE! Player " << other.getid() << " wins! " << endl;
							system("pause");
							system("cls");
							cout << " umar khan badozai " << endl;
							system("pause");
							return false;
						}
						else if (legalmove.isincheck(kingx, kingy, arr, tpieces, Tpieces, enemy, other, enpassantrow, enpassantcol))
						{
							cout << " ------------------------------ " << endl;
							cout << " |  player 1 CHECK! payer 2  |" << endl;
							cout << " ------------------------------ " << endl;
						}
						else if (legalmove.stalemate(kingx, kingy, arr, tpieces, Tpieces, enemy, other, enpassantrow, enpassantcol))
						{
							cout << " STALEMATE! DRAW! " << endl;
							return false;
						}
					}
					else
					{
						system("pause");
						system("cls");
						other.display();
						center();
						cout << " ended " << endl;
					}

					return true;
				}
				else
				{
					cout << " il-legal move " << endl;
				}
			}
			else
			{
				cout << " invalid move " << endl;
			}
		}

		// ── PLAYER 2 TURN ──
		if (other.getid() == 2)
		{
			bool found = false;
			for (int i = 0; i < 6; ++i)
			{
				if (tpieces[i] == arr[tix][tiy])
				{
					found = true;
				}
			}
			if (found)
			{
				bool legal = false;

				if (arr[tix][tiy] == 'p')
					legal = pawnmove.pawn2legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other, enpassantrow, enpassantcol);
				if (arr[tix][tiy] == 'r')
					legal = rookmove.rock1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);
				if (arr[tix][tiy] == 'b')
					legal = bishopmove.bishop1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);
				if (arr[tix][tiy] == 'n')
					legal = knightmove.knight1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);
				if (arr[tix][tiy] == 'q')
					legal = queenmove.queen1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);
				if (arr[tix][tiy] == 'k')
					legal = kingmove.king1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);

				if (legal)
					legal = legalmove.checklegal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other, enemy, enpassantrow, enpassantcol);

				if (legal)
				{
					arr[tfx][tfy] = arr[tix][tiy];
					arr[tix][tiy] = '-';

					// reset en passant every move
					enpassantrow = -1;
					enpassantcol = -1;

					// if pawn double stepped record position
					if (arr[tfx][tfy] == 'p' && tfx == tix - 2)
					{
						enpassantrow = tfx;
						enpassantcol = tfy;
					}

					// if en passant capture happened remove the captured pawn
					if (arr[tfx][tfy] == 'p' && tfx == tix - 1 && tfy != tiy && arr[tix][tfy] == 'P')
					{
						arr[tix][tfy] = '-';
					}

					bool swaplegal = pawnmove.swap1bool();
					if (swaplegal && arr[tfx][tfy] == 'p')
					{
						pawnmove.swap1pwan(arr, tfx, tfy, other);
					}

					// find enemy king position
					char enemyking = 'K';
					int kingx = -1, kingy = -1;

					for (int i = 0; i < 8; ++i)
					{
						for (int j = 0; j < 8; ++j)
						{
							if (arr[i][j] == enemyking)
							{
								kingx = i;
								kingy = j;
							}
						}
					}

					if (kingx != -1)
					{
						if (legalmove.checkmate(kingx, kingy, arr, tpieces, Tpieces, enemy, other, enpassantrow, enpassantcol))
						{
							system("cls");
							other.display();
							center();
							cout << " CHECKMATE! Player " << other.getid() << " wins! " << endl;
							system("pause");
							system("cls");
							cout << " umar khan badozai " << endl;
							system("pause");
							return false;
						}
						else if (legalmove.isincheck(kingx, kingy, arr, tpieces, Tpieces, enemy, other, enpassantrow, enpassantcol))
						{
							cout << " ------------------------------ " << endl;
							cout << " |  player 2 CHECK! player 1  |" << endl;
							cout << " ------------------------------ " << endl;
						}
						else if (legalmove.stalemate(kingx, kingy, arr, tpieces, Tpieces, enemy, other, enpassantrow, enpassantcol))
						{
							cout << " STALEMATE! DRAW! " << endl;
							return false;
						}
					}
					else
					{
						system("pause");
						system("cls");
						other.display();
						center();
						cout << "ended" << endl;
					}

					return true;
				}
				else
				{
					cout << " il-legal move " << endl;
				}
			}
			else
			{
				cout << " invalid move " << endl;
			}
		}
	}
}
board::~board()
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			arr[i][j] = '-';
		}
	}
}
char board::getPieceAt(int r, int c) {
    return arr[r][c];
}

bool board::hasPendingPromotion() const
{
    return pendingPromotionRow != -1 && pendingPromotionCol != -1;
}

board::MoveResult board::completePromotion(char promotedPiece, info& other, info& enemy)
{
    if (!hasPendingPromotion())
    {
        return MoveResult::Invalid;
    }

    if (other.getid() == 1)
    {
        if (promotedPiece != 'Q' && promotedPiece != 'R' && promotedPiece != 'B' && promotedPiece != 'N')
        {
            return MoveResult::Invalid;
        }
    }
    else
    {
        if (promotedPiece != 'q' && promotedPiece != 'r' && promotedPiece != 'b' && promotedPiece != 'n')
        {
            return MoveResult::Invalid;
        }
    }

    arr[pendingPromotionRow][pendingPromotionCol] = promotedPiece;
    pendingPromotionRow = -1;
    pendingPromotionCol = -1;

    char enemyKing = (other.getid() == 1) ? 'k' : 'K';
    int kingx = -1;
    int kingy = -1;

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (arr[i][j] == enemyKing)
            {
                kingx = i;
                kingy = j;
            }
        }
    }

    if (kingx != -1)
    {
        if (legalmove.checkmate(kingx, kingy, arr, tpieces, Tpieces, enemy, other, enpassantrow, enpassantcol))
        {
            return MoveResult::Checkmate;
        }
    }

    return MoveResult::Moved;
}

board::MoveResult board::moveSFML(int tix, int tiy, int tfx, int tfy, info& other, info& enemy)
{
    if (other.getid() == 1)
    {
        bool found = false;
        for (int i = 0; i < 6; ++i)
            if (Tpieces[i] == arr[tix][tiy])
                found = true;

        if (!found) return MoveResult::Invalid;

        bool legal = false;

        if (arr[tix][tiy] == 'P')
            legal = pawnmove.pawn1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other, enpassantrow, enpassantcol);
        if (arr[tix][tiy] == 'R')
            legal = rookmove.rock1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);
        if (arr[tix][tiy] == 'B')
            legal = bishopmove.bishop1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);
        if (arr[tix][tiy] == 'N')
            legal = knightmove.knight1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);
        if (arr[tix][tiy] == 'Q')
            legal = queenmove.queen1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);
        if (arr[tix][tiy] == 'K')
            legal = kingmove.king1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);

        if (legal)
            legal = legalmove.checklegal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other, enemy, enpassantrow, enpassantcol);

        if (legal)
        {
            arr[tfx][tfy] = arr[tix][tiy];
            arr[tix][tiy] = '-';

            enpassantrow = -1;
            enpassantcol = -1;

            if (arr[tfx][tfy] == 'P' && tfx == tix + 2)
            {
                enpassantrow = tfx;
                enpassantcol = tfy;
            }

            if (arr[tfx][tfy] == 'P' && tfx == tix + 1 && tfy != tiy && arr[tix][tfy] == 'p')
                arr[tix][tfy] = '-';

            bool swaplegal = pawnmove.swap1bool();
            if (swaplegal && arr[tfx][tfy] == 'P')
            {
                pendingPromotionRow = tfx;
                pendingPromotionCol = tfy;
                return MoveResult::PromotionPending;
            }

            int kingx = -1, kingy = -1;
            for (int i = 0; i < 8; ++i)
                for (int j = 0; j < 8; ++j)
                    if (arr[i][j] == 'k') { kingx = i; kingy = j; }

            if (kingx != -1)
            {
                if (legalmove.checkmate(kingx, kingy, arr, tpieces, Tpieces, enemy, other, enpassantrow, enpassantcol))
                    return MoveResult::Checkmate;
            }
            return MoveResult::Moved;
        }
        return MoveResult::Invalid;
    }

    if (other.getid() == 2)
    {
        bool found = false;
        for (int i = 0; i < 6; ++i)
            if (tpieces[i] == arr[tix][tiy])
                found = true;

        if (!found) return MoveResult::Invalid;

        bool legal = false;

        if (arr[tix][tiy] == 'p')
            legal = pawnmove.pawn2legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other, enpassantrow, enpassantcol);
        if (arr[tix][tiy] == 'r')
            legal = rookmove.rock1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);
        if (arr[tix][tiy] == 'b')
            legal = bishopmove.bishop1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);
        if (arr[tix][tiy] == 'n')
            legal = knightmove.knight1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);
        if (arr[tix][tiy] == 'q')
            legal = queenmove.queen1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);
        if (arr[tix][tiy] == 'k')
            legal = kingmove.king1legal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other);

        if (legal)
            legal = legalmove.checklegal(tix, tiy, tfx, tfy, arr, tpieces, Tpieces, other, enemy, enpassantrow, enpassantcol);

        if (legal)
        {
            arr[tfx][tfy] = arr[tix][tiy];
            arr[tix][tiy] = '-';

            enpassantrow = -1;
            enpassantcol = -1;

            if (arr[tfx][tfy] == 'p' && tfx == tix - 2)
            {
                enpassantrow = tfx;
                enpassantcol = tfy;
            }

            if (arr[tfx][tfy] == 'p' && tfx == tix - 1 && tfy != tiy && arr[tix][tfy] == 'P')
                arr[tix][tfy] = '-';

            bool swaplegal = pawnmove.swap1bool();
            if (swaplegal && arr[tfx][tfy] == 'p')
            {
                pendingPromotionRow = tfx;
                pendingPromotionCol = tfy;
                return MoveResult::PromotionPending;
            }

            int kingx = -1, kingy = -1;
            for (int i = 0; i < 8; ++i)
                for (int j = 0; j < 8; ++j)
                    if (arr[i][j] == 'K') { kingx = i; kingy = j; }

            if (kingx != -1)
            {
                if (legalmove.checkmate(kingx, kingy, arr, tpieces, Tpieces, enemy, other, enpassantrow, enpassantcol))
                    return MoveResult::Checkmate;
            }
            return MoveResult::Moved;
        }
        return MoveResult::Invalid;
    }
    return MoveResult::Invalid;
}
