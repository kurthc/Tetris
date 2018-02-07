#include "Game.h"

falling_piece::falling_piece()
{
	this->x = 0;
	this->y = 0;
	this->blocks = std::vector<int>
}

game_board::game_board()
{
	for (int y = 0; y < this->GameBoardHeight; ++y)
	{
		for (int x = 0; x < this->GameBoardWidth; ++x)
		{
			if (y == 0 || y == GameBoardHeight - 1 || x == 0 || x == GameBoardWidth - 1)
			{
				this->GameBoard[x][y] = 1;
			}
			else
			{
				this->GameBoard[x][y] = 0;
			}
			
		}
	}
	
}


game_state::game_state()
{

}

void game_state::SetStandardPieces()
{
	//this->StandardPiece[0].Blocks
}