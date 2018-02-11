#include "Game.h"

piece::piece()
{
	this->Center = intvec2(0, 0);
	//this->Blocks = {};
}

// Given the set of blocks in the first parameter, rotate them 90 degrees and put the result in the second parameter
void piece::RotateBlocks90(const std::vector<intvec2>& vIn, std::vector<intvec2>& vOut)
{
	// For now, just return the same thing.
	//std::vector<intvec2>::iterator it = vIn.begin();
	auto it = vIn.begin();
	while (it != vIn.end())
	{
		//vOut.push_back(*it);
		
		intvec2 RotatedLocation = intvec2(-1 * (*it).y, (*it).x);
		vOut.push_back(RotatedLocation);
		++it;
	}
}

void piece::GetRotatedPiecesFrom0()
{
	this->RotateBlocks90(Blocks[0], Blocks[1]);
	this->RotateBlocks90(Blocks[1], Blocks[2]);
	this->RotateBlocks90(Blocks[2], Blocks[3]);
}
