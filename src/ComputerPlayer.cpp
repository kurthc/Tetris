#include "Game.h"

void computer_player::RecalculateStrategy(const piece* CurrentPiece, const piece* NextPiece)
{
	game_board* GameBoardTemp = new game_board(this->GameRound->GameBoard);
	// sample data
	//this->StrategyOrientation = 1;
	//this->StrategyX = 2;

	double BestScore = -1.0;
	int BestOrientation = 0;
	int BestX = 0;

	std::ostringstream ss{ "" };

	for (int OIndex = 0; OIndex < 4; ++OIndex)
	{
		ss << "Orientation " << OIndex << ": ";
		int Left = CurrentPiece->GetLeft(OIndex);
		int Right = CurrentPiece->GetRight(OIndex);
		for (int XIndex = -Left; XIndex < GAME_BOARD_WIDTH - Right; ++XIndex)
		{
			falling_piece FP(*CurrentPiece);
			FP.PieceOrientation = OIndex;
			FP.CenterLocation = intvec2(XIndex, CurrentPiece->GetBottom(OIndex) + HEIGHT_OF_DEATH);
			FP.DropToBottom(GameBoardTemp);
			auto it = FP.CurrentBlocks().begin();
			while (it != FP.CurrentBlocks().end())
			{
				intvec2 FreezeLocation = *it + FP.CenterLocation;
				GameBoardTemp->GameBoard[FreezeLocation.y][FreezeLocation.x] = BitmapIndex::BlockBlue; //
				++it;
			}
			GameBoardTemp->ClearCompletedLines();
			double ThisBoardScore = this->MapScore(GameBoardTemp);
			ss << ThisBoardScore << " ";
			if (BestScore < 0 || ThisBoardScore < BestScore)
			{
				BestScore = ThisBoardScore;
				BestOrientation = OIndex;
				BestX = XIndex;
			}

			it = FP.CurrentBlocks().begin();
			while (it != FP.CurrentBlocks().end())
			{
				intvec2 FreezeLocation = *it + FP.CenterLocation;
				GameBoardTemp->GameBoard[FreezeLocation.y][FreezeLocation.x] = this->GameRound->GameBoard.GameBoard[FreezeLocation.y][FreezeLocation.x]; //
				++it;
			}
		}
		ss << std::endl;
	}
	
	this->StrategyOrientation = BestOrientation;
	this->StrategyX = BestX;

	ss << "Best Orientation: " << BestOrientation << ", Best X: " << BestX << std::endl << std::endl;
	std::cout << ss.str();
}


double computer_player::MapScore(game_board* GameBoard)
{
	return MaxHeightScore(GameBoard);
	//return HeightScore(GameBoard);
}

double computer_player::HeightScore(game_board* GameBoard)
{
	//
	int Score = 0;
	for (int x = 0; x < GAME_BOARD_WIDTH; ++x)
	{
		int ColumnHeight;
		ColumnHeight = 0;
		for (int y = HEIGHT_OF_DEATH; y >= 0; --y)
		{
			if (GameBoard->BlockHere(x,y))
			{
				ColumnHeight = y + 1;
			}
		}
		Score += ColumnHeight;
	}
	
	return (float)Score / (float)((HEIGHT_OF_DEATH +1) * GAME_BOARD_WIDTH);
}

double computer_player::MaxHeightScore(game_board* GameBoard)
{
	//
	int MaxHeight = 0;
	for (int x = 0; x < GAME_BOARD_WIDTH; ++x)
	{
		for (int y = HEIGHT_OF_DEATH; y >= -1; --y)
		{
			if (y == -1 || GameBoard->BlockHere(x, y))
			{
				MaxHeight = MAX(MaxHeight, y+1);
				break;
			}
		}
	}
	return (float)MaxHeight / (float)(HEIGHT_OF_DEATH + 1);
}


//double computer_player::HoleDensityScore(game_board* GameBoard)
//{
//
//}
