#include "Game.h"

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