#pragma once
#include <vector>

//#include "WindowsLayer.h"

constexpr int GAMEBOARDWIDTH = 10;
constexpr int GAMEBOARDHEIGHT = 20;
constexpr int GAMEBOARDPLAYABLEHEIGHT = GAMEBOARDHEIGHT + 4;

class game_board
{
public:
	const int GameBoardWidth = GAMEBOARDWIDTH;
	const int GameBoardHeight = GAMEBOARDHEIGHT;
	const int PlayableHeight = GAMEBOARDPLAYABLEHEIGHT;
	int GameBoard[GAMEBOARDWIDTH][GAMEBOARDPLAYABLEHEIGHT];
	game_board();
};

class falling_piece
{
public:
	int x;
	int y;
	std::vector<int> blocks;
	bool Visible = true;
	falling_piece();
};

enum piece_center_type { Center, Corner };

class piece
{
public:
	int CenterX;
	int CenterY;
	piece_center_type CenterType;
	std::vector<int> Blocks;
};

class game_state
{
public:
	game_board GameBoard;
	falling_piece FallingPiece;
	piece StandardPiece[1];

	game_state();
	void SetStandardPieces();
};

