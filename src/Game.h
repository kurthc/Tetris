#pragma once
#include <vector>
#include <iostream>
#include "Global.h"

class game_state;

constexpr int GAME_BOARD_WIDTH = 10;
constexpr int GAME_BOARD_HEIGHT = 20;
constexpr int GAME_BOARD_PLAYABLE_HEIGHT = GAME_BOARD_HEIGHT + 4;
constexpr float KEYBOARD_REPEAT_TIME = 0.1f;
constexpr float DROP_SPEED = 50.0f;

enum piece_center_type { Center, Corner };
enum player {User, Computer};

// class: game_board
// The blocks currently on the game board are stored in GameBoard[y][x], with y ordered bottom up:
//
//    2
//  y 1
//    0
//     0123
//      x
class game_board
{
public:
	const int GameBoardWidth = GAME_BOARD_WIDTH;
	const int GameBoardHeight = GAME_BOARD_HEIGHT;
	const int PlayableHeight = GAME_BOARD_PLAYABLE_HEIGHT;      // The highest point that blocks can be be dropped without triggering game over.
	int GameBoard[GAME_BOARD_PLAYABLE_HEIGHT][GAME_BOARD_WIDTH];
	game_board();
	void ClearBoard();
	BitmapIndex GetColor(int x, int y) const { return (BitmapIndex)GameBoard[y][x]; };
	void SetColor(int x, int y, BitmapIndex Color) { GameBoard[y][x] = Color; };
	bool BlockHere(int x, int y) const { return this->GetColor(x, y) != 0; };
};

// A tetris piece. Coordinates are not relative to the piece center.
// 
class piece
{
public:
	intvec2 Center{0,0};
	//piece_center_type CenterType;     // Currently not used. The idea was that pieces could be rotated about either a corner or a fixed block.
	std::vector<intvec2> Blocks[4];     // Index n corresponds to a rotation of 2PI*n/4
	BitmapIndex Color;

	void GetRotatedPiecesFrom0();
	void RotateBlocks90(const std::vector<intvec2>&, std::vector<intvec2>&);
	int GetBottom();
};

class falling_piece
{
public:
	intvec2 CenterLocation;
	bool Visible = true;
	int PieceOrientation = 0;
	piece* Piece;
	
	falling_piece() : Piece(nullptr) {}
	falling_piece(const piece&);
	bool HitSomething(const game_board&);
	intvec2 operator[](const int& n);
	std::vector<intvec2>& CurrentBlocks() { return this->Piece->Blocks[this->PieceOrientation]; }
	BitmapIndex Color() { return Piece->Color; }
	void ReplacePiece(piece*);
};


class computer_player
{
public:
	game_state* GameState;

	computer_player(game_state* GameState) : GameState(GameState) {};
	double MapScore();

};

class game_state
{
public:
	game_board GameBoard;
	falling_piece FallingPiece;
	piece* NextPiece;
	piece StandardPiece[7];
	int StandardPieceCount;
	bool ShowDebugOverlay = false;
	float DropTimer = 0.0f;
	bool UserIsPressingDown = false;
	bool GameOver = false;
	float FPSObserved = 0.0f;
	int Level = 1;
	int Score = 0;
	int LineCount = 0;
	float FallSpeed = 5.0f;
	player Player = player::User;
	computer_player* ComputerPlayer;

	game_state();
	void SetStandardPieces();
	void HandleKeyboard(keyboard_info*);
	void UpdateGame(keyboard_info*);
	void FreezePiece();
	void NewFallingPieceAtTop();
	void AddNextPiece();
	void ProcessFallingPiece();
	void ProcessLinesAfterDrop();
	void UpdateLevel();
	void HandleComputerKeyboard();
};




enum StandardPieceName { O = 0, I = 1, T = 2, L = 3, J = 4, S = 5, Z = 6 };

const int StandardPieceBlockData[7][4][2] = {
	{ { 0,0 },{  1,0 },{  1,1 },{  0,1 } },  // O
	{ { 0,0 },{ -1,0 },{  1,0 },{  2,0 } },  // I
	{ { 0,0 },{  1,0 },{ -1,0 },{  0,1 } },  // T
	{ { 0,0 },{  1,0 },{ -1,0 },{  1,1 } },  // L
	{ { 0,0 },{  1,0 },{ -1,0 },{ -1,1 } },  // J
	{ { 0,0 },{ -1,0 },{  0,1 },{  1,1 } },  // S
	{ { 0,0 },{  1,0 },{  0,1 },{ -1,1 } },  // Z
} ;
//
const static BitmapIndex StandardPieceColor[7] = {
	BitmapIndex::BlockWhite,  // O
	BitmapIndex::BlockYellow, // I
	BitmapIndex::BlockCyan,   // T
	BitmapIndex::BlockRed,    // L
	BitmapIndex::BlockBlue,   // J
	BitmapIndex::BlockGreen,  // S
	BitmapIndex::BlockPurple  // Z
};
