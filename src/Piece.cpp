#include "Game.h"

//piece::piece()
//{
//	this->Center = intvec2(0, 0);
//}

// Given the set of blocks in the first parameter, rotate them 90 degrees and put the result in the second parameter.
// Used to set the three other rotated forms, given the initial one.
void piece::RotateBlocks90(const std::vector<intvec2>& vIn, std::vector<intvec2>& vOut)
{
	auto it = vIn.begin();
	while (it != vIn.end())
	{
		// Rotate 90 degrees counterclockwise:  [x] -> [-y]
		//                                      [y]    [ x]
		intvec2 RotatedLocation = intvec2(-1 * (*it).y, (*it).x);
		vOut.push_back(RotatedLocation);
		++it;
	}
}

// Fill Blocks[1], Blocks[2], Blocks[3] from Blocks[0].
void piece::GetRotatedPiecesFrom0()
{
	this->RotateBlocks90(Blocks[0], Blocks[1]);
	this->RotateBlocks90(Blocks[1], Blocks[2]);
	this->RotateBlocks90(Blocks[2], Blocks[3]);
}

// Get the y value of the lowest block.
int piece::GetBottom()
{
	auto it = this->Blocks->begin();
	int Low = 100;
	while (it != this->Blocks->end())
	{
		if (it->y < Low)
			Low = it->y;
		++it;
	}
	return Low;
}

falling_piece::falling_piece(const piece& Piece)
{
	//this->Piece = Piece;    //copy
	this->Piece = new piece(Piece);
	int Height = this->Piece->GetBottom();
	this->CenterLocation = intvec2(4, Height + GAME_BOARD_HEIGHT);
	this->PieceOrientation = 0;
}


bool falling_piece::HitSomething(const game_board& GameBoard)
{
	bool IsOverlapping = false;

	for (int i = 0; i < this->CurrentBlocks().size(); ++i)
	{
		// For each block in the piece, check if it goes out of bounds...
		intvec2 b = (*this)[i];
		if (b.x < 0 || b.x >= GAME_BOARD_WIDTH || b.y < 0)
		{
			IsOverlapping = true;
		}
		// ...or if it hits another block.
		else if (GameBoard.BlockHere(b.x, b.y))
		{
			IsOverlapping = true;
		}
	}


	return IsOverlapping;
}

// GameMap coordinates of the nth block.
intvec2 falling_piece::operator[](const int& n)
{
	return (this->CurrentBlocks())[n] + this->CenterLocation;
}

void falling_piece::ReplacePiece(piece* NewPiece)
{
	if (this->Piece)
	{
		delete this->Piece;
	}
	this->Piece = NewPiece;
	return;
}