#pragma once

//#include "WindowsLayer.h"

constexpr int GAMEBOARDWIDTH = 10;
constexpr int GAMEBOARDHEIGHT = 20;

class game_board
{
public:
	const int GameBoardWidth = GAMEBOARDWIDTH;
	const int GameBoardHeight = GAMEBOARDHEIGHT;
	int GameBoard[GAMEBOARDWIDTH][GAMEBOARDHEIGHT];
	game_board();
};

class game_state
{
public:
	game_board GameBoard;
};