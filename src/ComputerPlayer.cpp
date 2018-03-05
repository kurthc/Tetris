#include "Game.h"

void computer_player::RecalculateStrategy(const piece* CurrentPiece, const piece* NextPiece)
{
	game_board* GameBoardTemp = new game_board(this->GameRound->GameBoard);
	// sample data
	//this->StrategyOrientation = 1;
	//this->StrategyX = 2;

	for (int OIndex = 0; OIndex < 4; ++OIndex)
	{
		int Left = CurrentPiece->GetLeft(OIndex);
		int Right = CurrentPiece->GetRight(OIndex);
		for (int XIndex = -Left; XIndex < GAME_BOARD_WIDTH - Right; ++XIndex)
		{
			falling_piece FP(*CurrentPiece);
			FP.PieceOrientation = OIndex;
			FP.CenterLocation = intvec2(XIndex, CurrentPiece->GetBottom(OIndex) + GAME_BOARD_HEIGHT);
			//this->GameRound->DropPiece();

			//GameBoardTemp->
			//for (auto it = CurrentPiece->Blocks[OIndex].begin(); it != CurrentPiece->Blocks[OIndex].end(); ++it)
			//{
			//	GameBoardTemp->GameBoard[]
			//}

			// after calculation:
			//GameBoardTemp->CopyBoard(&this->GameRound->GameBoard);
		}
	}
}



double computer_player::MapScore()
{
	//
	int Score = 0;
	for (int x = 0; x < GAME_BOARD_WIDTH; ++x)
	{
		for (int y = GAME_BOARD_HEIGHT; y >= 0; ++y)
		{
			if (this->GameRound->GameBoard.BlockHere(x,y))
			{
				Score = Score + y;
			}
		}
	}
	return (float)Score;
}