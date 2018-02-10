#pragma once
#include <vector>
#include <iostream>
#include "Global.h"

constexpr int GAMEBOARDWIDTH = 10;
constexpr int GAMEBOARDHEIGHT = 20;
constexpr int GAMEBOARDPLAYABLEHEIGHT = GAMEBOARDHEIGHT + 4;

enum piece_center_type { Center, Corner };



class game_board
{
public:
	const int GameBoardWidth = GAMEBOARDWIDTH;
	const int GameBoardHeight = GAMEBOARDHEIGHT;
	const int PlayableHeight = GAMEBOARDPLAYABLEHEIGHT;
	int GameBoard[GAMEBOARDWIDTH][GAMEBOARDPLAYABLEHEIGHT];
	game_board();
};





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
	
	//falling_piece(piece Piece) : Piece(Piece), CenterLocation(intvec2()) {};
	falling_piece();
	falling_piece(piece Piece);
};

class game_state
{
public:
	game_board GameBoard;
	falling_piece FallingPiece;
	piece StandardPiece[7];

	game_state();
	void SetStandardPieces();
	//void UpdateGame(key);
};

