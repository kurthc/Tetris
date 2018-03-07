#pragma once
#include <vector>
#include <iostream>
#include "Global.h"

class game_state;
class falling_piece;

constexpr int GAME_BOARD_WIDTH = 10;
constexpr int GAME_BOARD_HEIGHT = 20;
constexpr int GAME_BOARD_PLAYABLE_HEIGHT = GAME_BOARD_HEIGHT + 4;
constexpr float KEYBOARD_REPEAT_TIME = 0.1f;
constexpr float DROP_SPEED = 50.0f;

enum piece_center_type { Center, Corner };
enum player {User, Computer};


// A tetris piece. Coordinates are not relative to the piece center.
// 
class piece
{
public:
	intvec2 Center{0,0};
	//piece_center_type CenterType;     // Currently not used. The idea was that pieces could be rotated about either a corner or a fixed block.
	std::vector<intvec2> Blocks[4];     // Index n corresponds to a rotation of 2PI*n/4
	enum BoundDirection{Bottom, Top, Left, Right, Count};
	int Bound[4][BoundDirection::Count];
	BitmapIndex Color;

	//piece() :Center(0, 0) {};

	void GetRotatedPiecesFrom0();
	void RotateBlocks90(const std::vector<intvec2>&, std::vector<intvec2>&);
	void FillBounds();
	int GetBottom(const int n) const { return Bound[n][BoundDirection::Bottom]; }
	int GetTop(const int n) const { return Bound[n][BoundDirection::Top]; }
	int GetLeft(const int n) const { return Bound[n][BoundDirection::Left]; }
	int GetRight(const int n) const { return Bound[n][BoundDirection::Right]; }
};

// class: game_board
// The blocks currently on the game board are stored in GameBoard[y][x], with y ordered bottom to top:
//
//    2
//  y 1
//    0
//     0123
//      x
class game_board
{
public:
	//TODO: Why do you need these constants?
	const int GameBoardWidth = GAME_BOARD_WIDTH;
	const int GameBoardHeight = GAME_BOARD_HEIGHT;
	const int PlayableHeight = GAME_BOARD_PLAYABLE_HEIGHT;      // The highest point that blocks can be be dropped without triggering game over.
	int GameBoard[GAME_BOARD_PLAYABLE_HEIGHT][GAME_BOARD_WIDTH];
	game_board();
	game_board(const game_board&);
	void ClearBoard();
	void CopyBoard(game_board* GameBoardToCopy);
	bool FreezePiece(const piece& Piece, const intvec2 CenterLocation, const int PieceOrientation, const BitmapIndex Color);
	BitmapIndex GetColor(const int x, const int y) const { return (BitmapIndex)GameBoard[y][x]; };
	void SetColor(const int x, const int y, const BitmapIndex Color) { GameBoard[y][x] = Color; };
	bool BlockHere(const int x, const int y) const { return this->GetColor(x, y) != 0; };
	//void DropPiece(falling_piece* FallingPiece);
};


class falling_piece
{
public:
	intvec2 CenterLocation;
	bool Visible = true;
	int PieceOrientation = 0;
	piece* Piece;
	
	//falling_piece() : Piece(nullptr) {}
	falling_piece(const piece&);
	bool HitSomething(const game_board*);
	intvec2 operator[](const int& n);
	std::vector<intvec2>& CurrentBlocks() { return this->Piece->Blocks[this->PieceOrientation]; }
	BitmapIndex Color() { return Piece->Color; }
	void ReplacePiece(piece*);
	void DropToBottom(const game_board*);
};




class game_round
{
public:
	game_board GameBoard;
	falling_piece* FallingPiece;
	piece* NextPiece;
	float DropTimer = 0.0f;
	
	game_state* GameState;

	game_round(game_state* GameState);
	void AddNextPiece();
	void NewFallingPieceAtTop();
	void FreezePiece();
	void DropPiece();
	~game_round();
};

class computer_player
{
public:
	game_round* GameRound;
	int StrategyOrientation;
	int StrategyX;

	computer_player(game_round* GameRound) : GameRound(GameRound) {};
	void RecalculateStrategy(const piece* CurrentPiece, const piece* NextPiece);
	//void FindBestLocation();
	double MapScore();

};

class game_state
{
public:
	game_round* GameRound[1];
	piece StandardPiece[7];
	int StandardPieceCount;
	bool ShowDebugOverlay = false;
	bool UserIsPressingDown = false;
	bool GameOver = false;
	float FPSObserved = 0.0f;
	int Level = 1;
	int Score = 0;
	int LineCount = 0;
	float FallSpeed = 5.0f;
	player Player = player::User;
	computer_player* ComputerPlayer;
	float ComputerPlayerTimer = 0.0f;

	game_state();
	void SetStandardPieces();
	void HandleKeyboard(keyboard_info*);
	void UpdateGame(keyboard_info*);
	void ProcessFallingPiece(game_round* GameRound);
	void ProcessLinesAfterDrop(game_round* GameRound);
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
