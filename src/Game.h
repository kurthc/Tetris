#pragma once
#include <vector>
#include <iostream>

//#include "WindowsLayer.h"

constexpr int GAMEBOARDWIDTH = 10;
constexpr int GAMEBOARDHEIGHT = 20;
constexpr int GAMEBOARDPLAYABLEHEIGHT = GAMEBOARDHEIGHT + 4;

class intvec2
{
public:
	int x;
	int y;
	intvec2() : x(0), y(0) {}
	intvec2(int x, int y) : x(x), y(y) {}
};

class game_board
{
public:
	const int GameBoardWidth = GAMEBOARDWIDTH;
	const int GameBoardHeight = GAMEBOARDHEIGHT;
	const int PlayableHeight = GAMEBOARDPLAYABLEHEIGHT;
	int GameBoard[GAMEBOARDWIDTH][GAMEBOARDPLAYABLEHEIGHT];
	game_board();
};


enum piece_center_type { PieceCenterTypeCenter, PieceCenterTypeCorner };

class piece
{
public:
	intvec2 Center;
	piece_center_type CenterType;
	std::vector<intvec2> Blocks;
	piece();
};

class falling_piece
{
public:
	intvec2 CenterLocation;
	piece Piece;
	bool Visible = true;
	falling_piece(piece);
};

class game_state
{
public:
	game_board GameBoard;
	//falling_piece FallingPiece;
	piece StandardPiece[1];

	game_state();
	void SetStandardPieces();
};

