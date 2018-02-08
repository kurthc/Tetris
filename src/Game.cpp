#include "Game.h"

//falling_piece::falling_piece()
//{
//	this->x = 0;
//	this->y = 0;
//	//this->blocks = std::vector<int>
//}

intvec2 intvec2::operator+(const intvec2& v)
{
	int ResultX = this->x + v.x;
	int ResultY = this->y + v.y;
	return { ResultX, ResultY };
}

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

falling_piece::falling_piece()
{
	
}

falling_piece::falling_piece(piece Piece)
{
	this->Piece = Piece;    //copy
	this->CenterLocation = intvec2(4, 17);
	
}

game_state::game_state()
{
	this->SetStandardPieces();
	this->FallingPiece = falling_piece(this->StandardPiece[1]);
}

void game_state::SetStandardPieces()
{
	//piece Piece {};
	this->StandardPiece[0].Blocks.push_back(intvec2(0, 0));
	this->StandardPiece[0].Blocks.push_back(intvec2(1, 0));
	this->StandardPiece[0].Blocks.push_back(intvec2(1, 1));
	this->StandardPiece[0].Blocks.push_back(intvec2(0, 1));
	this->StandardPiece[0].Center = intvec2(0, 0);
	this->StandardPiece[0].CenterType = piece_center_type::Center;

	this->StandardPiece[1].Blocks.push_back(intvec2(0, 0));
	this->StandardPiece[1].Blocks.push_back(intvec2(1, 0));
	this->StandardPiece[1].Blocks.push_back(intvec2(-1, 0));
	this->StandardPiece[1].Blocks.push_back(intvec2(0, 1));
	this->StandardPiece[1].Center = intvec2(0, 0);
	this->StandardPiece[1].CenterType = piece_center_type::Center;



	//this->StandardPiece[0].Blocks
}