game_board::game_board()
{
	this->ClearBoard();
}


game_board::game_board(const game_board& InputGameBoard)
{
	// Make a copy of the GameBoard array.
	for (int y = 0; y < GAME_BOARD_HEIGHT; ++y)
	{
		for (int x = 0; x < GAME_BOARD_WIDTH; ++x)
		{
			this->GameBoard[y][x] = InputGameBoard.GameBoard[y][x];
		}
	}
}


void game_board::ClearBoard()
{
	for (int y = 0; y < GAME_BOARD_HEIGHT; ++y)
	{
		for (int x = 0; x < GAME_BOARD_WIDTH; ++x)
		{
			this->GameBoard[y][x] = 0;
		}
	}
}

void game_board::CopyBoard(game_board* GameBoardToCopy)
{
	//TODO: Would it be better to do this with memcpy?
	for (int y = 0; y < GAME_BOARD_HEIGHT; ++y)
	{
		for (int x = 0; x < GAME_BOARD_WIDTH; ++x)
		{
			this->GameBoard[y][x] = GameBoardToCopy->GameBoard[y][x];
		}
	}
}


bool game_board::UnderLineOfDeath(const intvec2 Location) const
{
	return this->UnderLineOfDeath(Location.x, Location.y);
}

bool game_board::UnderLineOfDeath(const int x, const int y) const
{
	return y < HEIGHT_OF_DEATH;
}


bool game_board::InGameBoardRegion(const intvec2 Location) const
{
	return this->InGameBoardRegion(Location.x, Location.y);
}

bool game_board::InGameBoardRegion(const int x, const int y) const
{
	return x >= 0 && x < GAME_BOARD_WIDTH && y >= 0 && y < GAME_BOARD_HEIGHT;
}


bool game_board::BlockHereOrOutOfBounds(const int x, const int y) const
{
	return !this->InGameBoardRegion(x, y) || this->BlockHere(x, y);
}

bool game_board::BlockHereOrOutOfBounds(const intvec2 Location) const
{
	return this->BlockHereOrOutOfBounds(Location.x, Location.y);
}


BitmapIndex game_board::GetColor(const int x, const int y) const
{
	assert(this->InGameBoardRegion(x, y));
	return (BitmapIndex)GameBoard[y][x];
}

// Add the blocks of a piece to the game_board data.
bool game_board::FreezePiece(const piece& Piece, const intvec2 CenterLocation, const int PieceOrientation, const BitmapIndex Color)
{
	bool BlockAboveLineOfDeath = false;
	auto it = Piece.Blocks[PieceOrientation].begin();
	while (it != Piece.Blocks[PieceOrientation].end())
	{
		intvec2 BlockLocation = CenterLocation + *it;
		if (0 <= BlockLocation.x && BlockLocation.x < GAME_BOARD_WIDTH
			&& 0 <= BlockLocation.y && BlockLocation.x < HEIGHT_OF_DEATH)
		{
			this->SetColor(BlockLocation.x, BlockLocation.y, Color);
		}
		if (BlockLocation.y >= HEIGHT_OF_DEATH)
		{
			BlockAboveLineOfDeath = true;
		}
		++it;
	}
	return BlockAboveLineOfDeath;
}


int game_board::ClearCompletedLines()
{
	int LinesMade = 0;

	// Search from top to bottom for completed rows.
	for (int BlockY = HEIGHT_OF_DEATH - 1; BlockY >= 0; --BlockY)
	{
		bool FoundHole = false;
		for (int BlockX = 0; BlockX < GAME_BOARD_WIDTH; ++BlockX)
		{
			if (!this->BlockHere(BlockX, BlockY))
			{
				FoundHole = true;
				break;
			}
		}

		if (!FoundHole)
		{
			// A row is complete. Drop all the blocks above it.
			for (int DroppingBlockY = BlockY; DroppingBlockY < GAME_BOARD_HEIGHT - 1; ++DroppingBlockY)
			{
				for (int DroppingBlockX = 0; DroppingBlockX < GAME_BOARD_WIDTH; ++DroppingBlockX)
				{
					// Set each block to the block above it.
					this->SetColor(DroppingBlockX, DroppingBlockY, this->GetColor(DroppingBlockX, DroppingBlockY + 1));
				}

			}

			// Clear the top row.
			for (int DroppingBlockX = 0; DroppingBlockX < GAME_BOARD_WIDTH; ++DroppingBlockX)
			{
				this->SetColor(DroppingBlockX, GAME_BOARD_HEIGHT - 1, BitmapIndex::BlockBlack);
			}

			++LinesMade;
		}
	}

	return LinesMade;
}