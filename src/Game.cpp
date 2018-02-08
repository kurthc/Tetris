#include "Game.h"

//falling_piece::falling_piece()
//{
//	this->x = 0;
//	this->y = 0;
//	//this->blocks = std::vector<int>
//}

piece::piece()
{
	this->Center = intvec2(0,0);
	this->Blocks = {};
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

falling_piece::falling_piece(piece Piece)
{
	this->Piece = Piece;    //copy
	this->CenterLocation = intvec2(4, 18);
	
}

game_state::game_state()
{
	this->SetStandardPieces();
}

void game_state::SetStandardPieces()
{
	//piece Piece {};
	this->StandardPiece[0].Blocks.push_back(intvec2(0, 0));
	this->StandardPiece[0].Blocks.push_back(intvec2(1, 0));
	this->StandardPiece[0].Blocks.push_back(intvec2(1, 1));
	this->StandardPiece[0].Blocks.push_back(intvec2(0, 1));
	this->StandardPiece[0].Center = intvec2(0, 0);
	this->StandardPiece[0].CenterType = PieceCenterTypeCenter;
	

	//std::cout << "SetStandardPieces";
	//this->StandardPiece[0].Blocks
}