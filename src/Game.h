#pragma once
#include <vector>
#include <iostream>
#include "Global.h"

constexpr int GAMEBOARDWIDTH = 10;
constexpr int GAMEBOARDHEIGHT = 20;
constexpr int GAMEBOARDPLAYABLEHEIGHT = GAMEBOARDHEIGHT + 4;
constexpr float KEYBOARD_REPEAT_TIME = .1f;

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

// A tetris piece. Coordinates are not relative to the game map.
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
	void HandleKeyboard(keyboard_info*);
	void UpdateGame(keyboard_info*);
};

