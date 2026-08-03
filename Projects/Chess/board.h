#pragma once
#include <iostream>
#include <iomanip>
#include "pieces.h"
#include "Pawn.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Queen.h"
#include "King.h"

using namespace std;

class info;

class board
{
public:
    enum class MoveResult
    {
        Invalid,
        Moved,
        PromotionPending,
        Checkmate
    };

private:
    const int x;
    const int y;

    pieces legalmove;
    Pawn   pawnmove;
    Rook   rookmove;
    Bishop bishopmove;
    Knight knightmove;
    Queen  queenmove;
    King   kingmove;

    char tpieces[6];
    char Tpieces[6];

    string srows, scolumns;

    int enpassantrow;
    int enpassantcol;
    int pendingPromotionRow;
    int pendingPromotionCol;

public:
    char arr[8][8];

    board();

    void createboard();
    void allotpices();
    void display();
    void center();

    char getPieceAt(int r, int c);
    bool move(info& other, info& enemy);
    MoveResult moveSFML(int fromRow, int fromCol, int toRow, int toCol,
        info& other, info& enemy);
    bool hasPendingPromotion() const;
    MoveResult completePromotion(char promotedPiece, info& other, info& enemy);
    ~board();
};
