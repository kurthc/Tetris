#pragma once
#include <vector>
#include <iostream>
#include "Global.h"

constexpr int GAME_BOARD_WIDTH = 10;
constexpr int GAME_BOARD_HEIGHT = 20;
constexpr int GAME_BOARD_PLAYABLE_HEIGHT = GAME_BOARD_HEIGHT + 4;
constexpr float KEYBOARD_REPEAT_TIME = .1f;

enum piece_center_type { Center, Corner };

class game_board
{
public:
	const int GameBoardWidth = GAME_BOARD_WIDTH;
	const int GameBoardHeight = GAME_BOARD_HEIGHT;
	const int PlayableHeight = GAME_BOARD_PLAYABLE_HEIGHT;
	int GameBoard[GAME_BOARD_WIDTH][GAME_BOARD_PLAYABLE_HEIGHT];
	game_board();
};

// A tetris piece. Coordinates are not relative to the piece center.
// 
class piece
{
public:
	intvec2 Center;
	piece_center_type CenterType;
	std::vector<intvec2> Blocks[4];

	piece();
	void GetRotatedPiecesFrom0();
	void RotateBlocks90(const std::vector<intvec2>&, std::vector<intvec2>&);
	int GetBottom();
};

class falling_piece
{
public:
	intvec2 CenterLocation;
	piece Piece;
	bool Visible = true;
	int PieceOrientation = 0;
	
	falling_piece();
	falling_piece(piece Piece);
	intvec2 BlockPosition(int n);
	bool HitSomething(const game_board&);
	intvec2 operator[](const int& n);
	std::vector<intvec2>& Blocks() { return this->Piece.Blocks[this->PieceOrientation]; }
};

class game_state
{
public:
	game_board GameBoard;
	falling_piece FallingPiece;
	piece StandardPiece[7];
	int StandardPieceCount;
	bool ShowDebugOverlay = false;
	float DropTimer = 0.0f;
	bool UserIsPressingDown = false;

	game_state();
	void SetStandardPieces();
	void HandleKeyboard(keyboard_info*);
	void UpdateGame(keyboard_info*);
	void FreezePiece();
	void NewFallingPieceAtTop();
	void ProcessFallingPiece();
};

