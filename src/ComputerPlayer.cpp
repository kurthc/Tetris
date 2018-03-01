#include "Game.h"


void computer_player::FindBestLocation()
{

}

double computer_player::MapScore()
{
	int Score = 0;
	for (int x = 0; x < GAME_BOARD_WIDTH; ++x)
	{
		for (int y = GAME_BOARD_HEIGHT; y >= 0; ++y)
		{
			if (this->GameBoard->BlockHere(x,y))
			{
				Score = Score + y;
			}
		}
	}
	return (float)Score;
}