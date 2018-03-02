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
//
//// Get the y value of the lowest block.
//int piece::GetBottom(int n)
//{
//	auto it = this->Blocks[n].begin();
//	int Low = 100;
//	while (it != this->Blocks[n].end())
//	{
//		if (it->y < Low)
//			Low = it->y;
//		++it;
//	}
//	return Low;
//}
//
////TODO: Combine GetLeft(), GetRight() somehow. 
//int piece::GetLeft(int n)
//{
//	int Left;
//	
//	//std::vector<intvec2> i = (this->Blocks)[n];
//	
//	if ((this->Blocks[n]).size() > 0)
//	{
//		auto it = this->Blocks[n].begin();
//		Left = (*it).x;
//		++it;
//		while (it != this->Blocks[n].end())
//		{
//			if ((*it).x < Left)
//				Left = (*it).x;
//			++it;
//		}
//	}
//	else
//	{
//		// TODO: Error
//		Left = 0;
//	}
//	
//	return Left;
//}
//
//
//int piece::GetRight(int n)
//{
//	int Right;
//	if (this->Blocks[n].size() > 0)
//	{
//		auto it = this->Blocks[n].begin();
//		Right = (*it).x;
//		++it;
//		while (it != this->Blocks[n].end())
//		{
//			if ((*it).x > Right)
//				Right = (*it).x;
//			++it;
//		}
//	}
//	else
//	{
//		// TODO: Error
//		Right = 0;
//	}
//
//	return Right;
//}

void piece::FillBounds()
{
	if ((this->Blocks[0].size() > 0) && (this->Blocks[1].size() > 0) && (this->Blocks[2].size() > 0) && (this->Blocks[3].size() > 0))
	{

		for (int Orientation = 0; Orientation < 4; ++Orientation)
		{
			//int BestBound[BoundDirection::Count];
			auto it = this->Blocks[Orientation].begin();
			//this->Bound[Orientation][BoundDirection::Bottom] = 
			this->Bound[Orientation][BoundDirection::Bottom] = this->Bound[Orientation][BoundDirection::Top] = (*it).y;
			this->Bound[Orientation][BoundDirection::Left] = this->Bound[Orientation][BoundDirection::Right] = (*it).x;
			++it;
			while (it != this->Blocks[Orientation].end())
			{
				this->Bound[Orientation][BoundDirection::Bottom] = MIN((*it).y, this->Bound[Orientation][BoundDirection::Bottom]);
				this->Bound[Orientation][BoundDirection::Top] = MAX((*it).y, this->Bound[Orientation][BoundDirection::Top]);
				this->Bound[Orientation][BoundDirection::Left] = MIN((*it).x, this->Bound[Orientation][BoundDirection::Left]);
				this->Bound[Orientation][BoundDirection::Right] = MAX((*it).x, this->Bound[Orientation][BoundDirection::Right]);
				++it;
			}

		}
	}
	else
	{
		// Something when wrong.

	}
}

falling_piece::falling_piece(const piece& Piece)
{
	//this->Piece = Piece;    //copy
	this->Piece = new piece(Piece);
	int Height = this->Piece->GetBottom(0);
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