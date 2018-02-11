#include "Game.h"

piece::piece()
{
	this->Center = intvec2(0, 0);
}

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


