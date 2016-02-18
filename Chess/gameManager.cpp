#include "gameManager.h"
#include <iostream>

gameManager::gameManager()
{
	gameOver = false;
	turn = "w";
	whiteMovelist.resize(0);
	blackMovelist.resize(0);

	movedFrom = NULL;
	movedTo = NULL;
	movedFromX = -1, movedFromY = -1, movedToX = -1, movedToY = -1;
	moved = false;

	movedFromOrig = NULL;
	movedToOrig = NULL;
	movedFromOrigX = -1, movedFromOrigY = -1, movedToOrigX = -1, movedToOrigY = -1;
}

gameManager::~gameManager()
{
}

/**
	TODO add promotion
	change checkpossiblemoves to check for type - use new helper method
**/

// Runs the entire chess game from start to finish
int gameManager::run()
{
	/********** START : Board initialization **********/

	// Sets the x and y coordinates of each square on the board to the correct value (defaults to -1, -1)
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			gameBoard[x][y].setXY(x, y);
		}
	}

	// Link all board squares together
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			// Top + bottom must be null
			if (y == 0)
			{
				directionSetterHelper('U', x, y);
			}
			else if (y == 7)
			{
				directionSetterHelper('D', x, y);
			}
			else
			{
				// UDLR
				gameBoard[x][y].setDirection('U', &gameBoard[x][y - 1]);
				gameBoard[x][y].setDirection('D', &gameBoard[x][y + 1]);

				// Left and right most end must be null
				if (x == 0) gameBoard[x][y].setDirection('L', NULL);
				else gameBoard[x][y].setDirection('L', &gameBoard[x - 1][y]);

				if (x == 7) gameBoard[x][y].setDirection('R', NULL);
				else gameBoard[x][y].setDirection('R', &gameBoard[x + 1][y]);
			}
		}
	}

	/********** END : Board initialization **********/

	/********** START : Chess piece initialization **********/

	// Initialize all the chess pieces
	// 1st half of the array is for white, 2nd half is for black
	pawns = new pawn[16];
	rooks = new rook[4];
	knights = new knight[4];
	bishops = new bishop[4];
	kings = new king[2];
	queens = new queen[2];

	/** Set the location of the chess pieces + occupants of each square on the board **/

	// Pawns
	for (int i = 0; i < 16; i++)
	{
		// White
		if (i < 8)
		{
			setPieceInfoAndLoc(-1, 6, i, "w", "pawn");
		}
		// Black
		else
		{
			setPieceInfoAndLoc(-1, 1, i, "b", "pawn");
		}
	}

	for (int i = 0; i < 4; i++)
	{
		// White
		if (i < 2)
		{
			/** Set pieces to their respective ends of the board **/
			/** White is on the bottom end of the board **/

			// Rooks
			if (i == 0)
			{
				setPieceInfoAndLoc(0, 7, i, "w", "rook");
			}
			else
			{
				setPieceInfoAndLoc(7, 7, i, "w", "rook");
			}

			// Knights
			if (i == 0)
			{
				setPieceInfoAndLoc(1, 7, i, "w", "knight");
			}
			else
			{
				setPieceInfoAndLoc(6, 7, i, "w", "knight");
			}

			// Bishops
			if (i == 0)
			{
				setPieceInfoAndLoc(2, 7, i, "w", "bishop");
			}
			else
			{
				setPieceInfoAndLoc(5, 7, i, "w", "bishop");
			}

		}
		// Black
		else
		{
			// Rooks
			if (i == 2)
			{
				setPieceInfoAndLoc(0, 0, i, "b", "rook");
			}
			else
			{
				setPieceInfoAndLoc(7, 0, i, "b", "rook");
			}

			// Knights
			if (i == 2)
			{
				setPieceInfoAndLoc(1, 0, i, "b", "knight");
			}
			else
			{
				setPieceInfoAndLoc(6, 0, i, "b", "knight");
			}

			// Bishops
			if (i == 2)
			{
				setPieceInfoAndLoc(2, 0, i, "b", "bishop");
			}
			else
			{
				setPieceInfoAndLoc(5, 0, i, "b", "bishop");
			}
		}
	}

	for (int i = 0; i < 2; i++)
	{
		// White
		if (i == 0)
		{
			// Queen
			setPieceInfoAndLoc(3, 7, i, "w", "queen");

			// King
			setPieceInfoAndLoc(4, 7, i, "w", "king");
		}
		// Black
		else
		{
			// Queen
			setPieceInfoAndLoc(3, 0, i, "b", "queen");

			// King
			setPieceInfoAndLoc(4, 0, i, "b", "king");
		}
	}

	/********** END : Chess piece initialization **********/

	checkPossibleMoves();

	//clearScreen();
	drawBoard();

	// Continue prompting the players for moves and switch player turns until the game ends
	while (!gameOver)
	{
		movePrompt(turn);

		if (turn.compare("w") == 0)
		{
			turn = "b";
		}
		else
		{
			turn = "w";
		}
	}

	return 1;
}

/**
Used to help link all the board squares and set the edges of the board to be NULL.

	@direction
		Direction of the link that is to be made
	@x
		x-coordinate of the square on the board
	@y
		y-coordinate of the square on the board
**/
void gameManager::directionSetterHelper(char direction, int x, int y)
{
	switch (direction)
	{
		// Top-most row of the board
	case 'U':
		gameBoard[x][y].setDirection('U', NULL);
		gameBoard[x][y].setDirection('D', &gameBoard[x][y + 1]);

		break;
		// Bottom-most row of the board
	case 'D':
		gameBoard[x][y].setDirection('D', NULL);
		gameBoard[x][y].setDirection('U', &gameBoard[x][y - 1]);
		break;
	}

	// Left and right most end must be null
	if (x == 0) gameBoard[x][y].setDirection('L', NULL);
	else gameBoard[x][y].setDirection('L', &gameBoard[x - 1][y]);

	if (x == 7) gameBoard[x][y].setDirection('R', NULL);
	else gameBoard[x][y].setDirection('R', &gameBoard[x + 1][y]);
}

/**
	Helper method that is used to set the required info for each chess piece.
**/
void gameManager::setPieceInfoAndLoc(int x, int y, int count, string color, string type)
{
	if (type.compare("pawn") == 0)
	{
		pawns[count].setColor(color);

		if (color.compare("w") == 0)
		{
			pawns[count].setXY(count, 6);
			pawns[count].setNum(count);
			gameBoard[count][6].setOccupant(&pawns[count]);
		}
		else if (color.compare("b") == 0)
		{
			pawns[count].setXY(count - 8, 1);
			pawns[count].setNum(count - 8);
			gameBoard[count - 8][1].setOccupant(&pawns[count]);
		}
	}
	else if (type.compare("rook") == 0)
	{
		rooks[count].setXY(x, y);
		gameBoard[x][y].setOccupant(&rooks[count]);
		rooks[count].setColor(color);

		if (count == 0 || count == 2)
		{
			rooks[count].setNum(0);
		}
		else if (count == 1 || count == 3)
		{
			rooks[count].setNum(1);
		}
	}
	else if (type.compare("knight") == 0)
	{
		knights[count].setXY(x, y);
		gameBoard[x][y].setOccupant(&knights[count]);
		knights[count].setColor(color);

		if (count == 0 || count == 2)
		{
			knights[count].setNum(0);
		}
		else if (count == 1 || count == 3)
		{
			knights[count].setNum(1);
		}
	}
	else if (type.compare("bishop") == 0)
	{
		bishops[count].setXY(x, y);
		gameBoard[x][y].setOccupant(&bishops[count]);
		bishops[count].setColor(color);

		if (count == 0 || count == 2)
		{
			bishops[count].setNum(0);
		}
		else if (count == 1 || count == 3)
		{
			bishops[count].setNum(1);
		}
	}
	else if (type.compare("king") == 0)
	{
		kings[count].setXY(x, y);
		gameBoard[x][y].setOccupant(&kings[count]);
		kings[count].setColor(color);
		kings[count].setNum(0);
	}
	else if (type.compare("queen") == 0)
	{
		queens[count].setXY(x, y);
		gameBoard[x][y].setOccupant(&queens[count]);
		queens[count].setColor(color);
		queens[count].setNum(0);
	}
}

/***** 
	Helper methods related to checking and setting posible moves for chess piece 
*****/

/**
	Checks and sets all possible moves for all chess pieces which are not dead
**/
void gameManager::checkPossibleMoves()
{
	int x_coord = -1;
	int y_coord = -1;
	boardSquare * tempSquare;
	
	whiteMovelist.resize(0);
	blackMovelist.resize(0);
	whiteMovelistSource.resize(0);
	blackMovelistSource.resize(0);

	clearAllPossibleMoves();

	// Pawns
	for (int i = 0; i < 16; i++)
	{
		// Ignore dead pieces
		if (pawns[i].isDead()) continue;

		x_coord = pawns[i].getXCoord();
		y_coord = pawns[i].getYCoord();
		tempSquare = &gameBoard[x_coord][y_coord];

		// White
		if (i < 8)
		{
			pawnRLUMoveCheck(x_coord, y_coord, "w", tempSquare, i);
		}
		// Black
		else
		{
			pawnRLUMoveCheck(x_coord, y_coord, "b", tempSquare, i);
		}
	}

	// Rooks
	for (int i = 0; i < 4; i++)
	{
		// Ignore dead pieces
		if (rooks[i].isDead()) continue;

		x_coord = rooks[i].getXCoord();
		y_coord = rooks[i].getYCoord();
		tempSquare = &gameBoard[x_coord][y_coord];

		// White
		if (i < 2)
		{
			rookAllDirectionsCheck("w", "rook", tempSquare, x_coord, y_coord, i);
		}
		// Black
		else
		{
			rookAllDirectionsCheck("b", "rook", tempSquare, x_coord, y_coord, i);
		}
	}

	// Knights
	for (int i = 0; i < 4; i++)
	{
		// Ignore dead pieces
		if (knights[i].isDead()) continue;

		x_coord = knights[i].getXCoord();
		y_coord = knights[i].getYCoord();
		tempSquare = &gameBoard[x_coord][y_coord];

		// White
		if (i < 2)
		{
			knightAllDirectionsCheck("w", i, tempSquare, x_coord, y_coord);
		}
		// Black
		else
		{
			knightAllDirectionsCheck("b", i, tempSquare, x_coord, y_coord);
		}
	}

	// Bishops
	for (int i = 0; i < 4; i++)
	{
		// Ignore dead pieces
		if (bishops[i].isDead()) continue;

		x_coord = bishops[i].getXCoord();
		y_coord = bishops[i].getYCoord();
		tempSquare = &gameBoard[x_coord][y_coord];

		// White
		if (i < 2)
		{
			bishopAllDirectionCheck("w", "bishop", tempSquare, x_coord, y_coord, i);
		}
		// Black
		else
		{
			bishopAllDirectionCheck("b", "bishop", tempSquare, x_coord, y_coord, i);
		}
	}

	// Kings
	for (int i = 0; i < 2; i++)
	{
		// Ignore dead pieces
		// Maybe this check isn't necessary. If the king is dead the game is over already
		if (kings[i].isDead()) continue;

		x_coord = kings[i].getXCoord();
		y_coord = kings[i].getYCoord();
		tempSquare = &gameBoard[x_coord][y_coord];

		// White
		if (i == 0)
		{
			kingAllDirectionsCheck("w", i, tempSquare, x_coord, y_coord);

		}
		// Black
		else
		{
			kingAllDirectionsCheck("b", i, tempSquare, x_coord, y_coord);
		}
	}

	// Queens << reuse rook + bishop moveset
	for (int i = 0; i < 2; i++)
	{
		// Ignore dead pieces
		if (queens[i].isDead()) continue;

		x_coord = queens[i].getXCoord();
		y_coord = queens[i].getYCoord();
		tempSquare = &gameBoard[x_coord][y_coord];

		// White
		if (i == 0)
		{
			rookAllDirectionsCheck("w", "queen", tempSquare, x_coord, y_coord, i);
			bishopAllDirectionCheck("w", "queen", tempSquare, x_coord, y_coord, i);
		}
		// Black
		else
		{
			rookAllDirectionsCheck("b", "queen", tempSquare, x_coord, y_coord, i);
			bishopAllDirectionCheck("b", "queen", tempSquare, x_coord, y_coord, i);
		}
	}
}

/**
Adds the given x+y coordinates as a possible move for the given chess piece

	@toAddX
		x-coordinate of the potential move that is to be added to the moveList of the chess piece
	@toAddY
		y-coordinate of the potential move that is to be added to the moveList of the chess piece
**/
void gameManager::setPossibleMove(string color, string type, int num, int toAddX, int toAddY, int x_coord, int y_coord)
{
	bool added = false;
	pair<int, int> toAdd;
	toAdd = make_pair(toAddX, toAddY);

	if (type.compare("pawn") == 0)
	{
		pawns[num].addPossibleMove(toAdd);
		added = true;
	}
	else if (type.compare("rook") == 0)
	{
		rooks[num].addPossibleMove(toAdd);
		added = true;
	}
	else if (type.compare("knight") == 0)
	{
		knights[num].addPossibleMove(toAdd);
		added = true;
	}
	else if (type.compare("bishop") == 0)
	{
		bishops[num].addPossibleMove(toAdd);
		added = true;
	}
	else if (type.compare("queen") == 0)
	{
		queens[num].addPossibleMove(toAdd);
		added = true;
	}
	else if (type.compare("king") == 0)
	{
		kings[num].addPossibleMove(toAdd);
		added = true;
	}

	// Add move to the respective black/white movelist (for checking checkmate status)
	if (color.compare("w") == 0)
	{
		whiteMovelist.push_back(toAdd);

		pair<int, int> source = make_pair(x_coord, y_coord);
		whiteMovelistSource.push_back(source);
	}
	else if (color.compare("b") == 0)
	{
		blackMovelist.push_back(toAdd);

		pair<int, int> source = make_pair(x_coord, y_coord);
		blackMovelistSource.push_back(source);
	}
}

/**
	Checks to see if the given board square is occupied by another chess piece.
	If not, it is added to the list of possible moves for the given chess piece.
	If occupied, it is only added to the list of possible moves if the occupant is an enemy chess piece

	@type
		Type of the chess piece (eg. rook/knight/bishop/etc...)
**/
int gameManager::tempSquareOccupantCheck(string color, string type, int num, boardSquare * tempSquare, int x_coord, int y_coord)
{
	genericPiece * tempPiece;
	// Color of the piece that is occupying a potential square that the rook can be moved to
	string tempColor = "";

	// Add to possible moves list if unoccupied
	if (!tempSquare->isOccupied())
	{
		setPossibleMove(color, type, num, tempSquare->getSquareXCoord(), tempSquare->getSquareYCoord(), x_coord, y_coord);
		return 0;
	}
	else
	{
		tempPiece = tempSquare->getOccupant();
		tempColor = tempPiece->getColor();

		// Add to possible moves list if occupied by an enemy chess piece
		if (color.compare("w") == 0 && tempColor.compare("b") == 0)
		{
			setPossibleMove(color, type, num, tempSquare->getSquareXCoord(), tempSquare->getSquareYCoord(), x_coord, y_coord);
		}
		else if (color.compare("b") == 0 && tempColor.compare("w") == 0)
		{
			setPossibleMove(color, type, num, tempSquare->getSquareXCoord(), tempSquare->getSquareYCoord(), x_coord, y_coord);
		}
		return 1;
	}
}

/**
	Clears the possible move list for all pieces
**/
void gameManager::clearAllPossibleMoves()
{
	for (int i = 0; i < 16; i++)
	{
		pawns[i].clearPossibleMoves();
	}
	for (int i = 0; i < 4; i++)
	{
		rooks[i].clearPossibleMoves();
		knights[i].clearPossibleMoves();
		bishops[i].clearPossibleMoves();
	}
	for (int i = 0; i < 2; i++)
	{
		queens[i].clearPossibleMoves();
		kings[i].clearPossibleMoves();
	}
}

/**
	Checks to see the king of the the specified player is checkmated
	Returns 0 if not, 1 if king is checked, and 2 if king is checkmated

	@player
		The color of the given player
**/
int gameManager::checkMateStatus(string player)
{
	int x_coord = -1;
	int y_coord = -1;
	int status = 0;
	vector< pair<int, int> > kingMovelist;
	vector<bool> threatened;
	int allThreatened = 0;
	bool kingPosThreatened = false;

	kingMovelist.resize(0);
	// Vector representing each possible move of the king
	// If the entry is TRUE, then the move will expose the king to check
	threatened.resize(0);

	// White
	if (player.compare("w") == 0)
	{
		// Check
		if (kingIsThreatened(player, kings[0].getXCoord(), kings[0].getYCoord()))
		{
			kingPosThreatened = true;
			status = 1;

			// Mate
			bool validMovesAvailable = anyValidMoves("w");

			if (validMovesAvailable == true)
			{
				return 1;
			}
			else if (validMovesAvailable == false)
			{
				return 2;
			}
		}
	}
	// Black
	else
	{
		// Check
		if (kingIsThreatened(player, kings[1].getXCoord(), kings[1].getYCoord()))
		{
			kingPosThreatened = true;
			status = 1;

			// Mate
			bool validMovesAvailable = anyValidMoves("b");

			if (validMovesAvailable == true)
			{
				return 1;
			}
			else if (validMovesAvailable == false)
			{
				return 2;
			}
		}
	}

	return 0;
}

/**
Checks to see if the king of the specified color would be threatened by any enemy pieces if it(the king) were at the coordinates given.
Returns TRUE if threatened, FALSE otherwise

	@color
		The color of the specified king
	@x_coord
		The x-coordinate of the position which is to be checked
	@y_coord
		The y-coordinate of the position which is to be checked
**/
bool gameManager::kingIsThreatened(string color, int x_coord, int y_coord)
{
	pair<int, int> toCheck;
	toCheck = make_pair(x_coord, y_coord);

	if (color.compare("w") == 0)
	{
		if (find(blackMovelist.begin(), blackMovelist.end(), toCheck) != blackMovelist.end())
		{
			// Threatened
			return true;
		}
	}
	else
	{
		if (find(whiteMovelist.begin(), whiteMovelist.end(), toCheck) != whiteMovelist.end())
		{
			// Threatened
			return true;
		}
	}

	return false;
}

/**
	Goes through the list of all possible moves that can be made by the given player to see if any of those moves will not result in a check.

	@playerTurn
		The color of the given player
**/
bool gameManager::anyValidMoves(string playerTurn)
{
	vector< pair<int, int> > moveList;
	vector< pair<int, int> > moveListSource;
	genericPiece * king = new genericPiece;

	if (playerTurn.compare("w") == 0)
	{
		moveList = whiteMovelist;
		moveListSource = whiteMovelistSource;
		king = &kings[0];
	}
	else if (playerTurn.compare("b") == 0)
	{
		moveList = blackMovelist;
		moveListSource = blackMovelistSource;
		king = &kings[1];
	}

	// Iterate through all possible moves here
	// Undo each move as we go through the list
	// If any single move does not lead to check, then it is only a check, not checkmate
	// If all moves are invalid moves, then the king is checkmated
	for (int i = 0; i < moveList.size(); i++)
	{
		int toMoveX = moveListSource[i].first;
		int toMoveY = moveListSource[i].second;

		boardSquare * toMoveSquare = &gameBoard[toMoveX][toMoveY];
		genericPiece * toMovePiece = toMoveSquare->getOccupant();
		string color = toMovePiece->getColor();
		string type = toMovePiece->getType();
		int num = toMovePiece->getNum();

		if (color.compare("b") == 0)
		{
			if (type.compare("pawn") == 0)
			{
				num += 8;
			}
			else if (type.compare("rook") == 0 || type.compare("knight") == 0 || type.compare("bishop") == 0)
			{
				num += 2;
			}
			else if (type.compare("king") == 0 || type.compare("queen") == 0)
			{
				num = 1;
			}
		}

		// No need to check if the move is valid
		// The move is guaranteed to be valid as we are pulling from a list of valid moves
		movePiece(type, num, moveList[i], false);

		checkPossibleMoves();
		if (!kingIsThreatened(color, king->getXCoord(), king->getYCoord()))
		{
			undoLastMove(false);
			return true;
		}

		undoLastMove(false);
	}

	return false;
}

/********** 
	Helper methods related to checking possible moves for each of the different chess piece types 
**********/

/**
	Checks for posible moves in all possible directions for a given pawn piece
**/
void gameManager::pawnRLUMoveCheck(int x_coord, int y_coord, string color, boardSquare * tempSquare, int num)
{
	// Skip diagonal check for pieces that are on the outermost(L/R) rows
	// Right diagonal (possible capture)
	if (x_coord != 7)
	{
		pawnPossibleDiagonalMoves("R", color, x_coord, y_coord, tempSquare, num);
	}

	// Left diagonal (possible capture)
	if (x_coord != 0)
	{
		pawnPossibleDiagonalMoves("L", color, x_coord, y_coord, tempSquare, num);
	}

	// White
	if (color.compare("w") == 0)
	{
		// Forward (Up)
		// Not allowed to move once the end of the board is reached
		if (y_coord != 0)
		{
			tempSquare = tempSquare->getDirection('U');

			if (!tempSquare->isOccupied())
			{
				// TODO add U to possible moves
				setPossibleMove(color, "pawn", num, tempSquare->getSquareXCoord(), tempSquare->getSquareYCoord(), x_coord, y_coord);
			}

			// Reset tempSquare for the double square check
			tempSquare = &gameBoard[x_coord][y_coord];
			// Double square move (first move)
			if (y_coord == 6 && !(tempSquare->getDirection('U'))->isOccupied() && !((tempSquare->getDirection('U'))->getDirection('U'))->isOccupied())
			{
				tempSquare = tempSquare->getDirection('U');
				tempSquare = tempSquare->getDirection('U');
				setPossibleMove(color, "pawn", num, tempSquare->getSquareXCoord(), tempSquare->getSquareYCoord(), x_coord, y_coord);
			}
		}
	}
	// Black
	else if (color.compare("b") == 0)
	{
		// Forward (Down)
		// Not allowed to move once the end of the board is reached
		if (y_coord != 7)
		{
			tempSquare = tempSquare->getDirection('D');

			if (!tempSquare->isOccupied())
			{
				setPossibleMove(color, "pawn", num, tempSquare->getSquareXCoord(), tempSquare->getSquareYCoord(), x_coord, y_coord);
			}

			// Reset tempSquare for the double square check
			tempSquare = &gameBoard[x_coord][y_coord];
			// Double square move (first move)
			if (y_coord == 1 && !(tempSquare->getDirection('D'))->isOccupied() && !((tempSquare->getDirection('D'))->getDirection('D'))->isOccupied())
			{
				tempSquare = tempSquare->getDirection('D');
				tempSquare = tempSquare->getDirection('D');
				setPossibleMove(color, "pawn", num, tempSquare->getSquareXCoord(), tempSquare->getSquareYCoord(), x_coord, y_coord);
			}
		}
	}
}

/**
	Checks for possible diagonal capture moves for the given pawn piece

	@direction
		The diagonal direction in which the pawn will move (eg. Right for Upper Right, etc...)
**/
void gameManager::pawnPossibleDiagonalMoves(string direction, string color, int x, int y, boardSquare * tempSquare, int num)
{
	//boardSquare * tempSquare;
	genericPiece * tempPiece;
	string tempColor = "";

	// Left or right diagonal move
	if (direction.compare("R") == 0)
	{
		tempSquare = tempSquare->getDirection('R');
	}
	else if (direction.compare("L") == 0)
	{
		tempSquare = tempSquare->getDirection('L');
	}

	// White moves up, black moves down
	if (color.compare("w") == 0)
	{
		tempSquare = tempSquare->getDirection('U');
	}
	else if (color.compare("b") == 0)
	{
		tempSquare = tempSquare->getDirection('D');
	}

	// If we have reached the end of the board, stop
	if (tempSquare == NULL)
	{
		return;
	}

	// If a forward diagonal square is occupied
	// Can't use the standard helper method used for the others, because the pawn can only move diagonally if the square is occupied by an enemy piece
	if (tempSquare->isOccupied())
	{
		tempPiece = tempSquare->getOccupant();
		tempColor = tempPiece->getColor();

		// Diagonal moves only allowed for capturing a piece of the opposing color
		if (color.compare("w") == 0 && tempColor.compare("b") == 0)
		{
			setPossibleMove(color, "pawn", num, tempSquare->getSquareXCoord(), tempSquare->getSquareYCoord(), x, y);
		}
		else if (color.compare("b") == 0 && tempColor.compare("w") == 0)
		{
			setPossibleMove(color, "pawn", num, tempSquare->getSquareXCoord(), tempSquare->getSquareYCoord(), x, y);
		}
	}
}

/**
	Checks for posible moves in all possible directions for given a rook piece
	This function is reused for checking the queen's possible moves

	@type
		String indicating the type of the chess piece (rook/queen)
**/
void gameManager::rookAllDirectionsCheck(string color, string type, boardSquare * tempSquare, int x_coord, int y_coord, int num)
{
	// Up
	if (y_coord > 0)
	{
		rookDirectionMoveCheck(color, type, 'U', tempSquare, x_coord, y_coord, num);
	}
	// Right
	if (x_coord < 7)
	{
		rookDirectionMoveCheck(color, type, 'R', tempSquare, x_coord, y_coord, num);
	}
	// Down
	if (y_coord < 7)
	{
		rookDirectionMoveCheck(color, type, 'D', tempSquare, x_coord, y_coord, num);
	}
	// Left
	if (x_coord > 0)
	{
		rookDirectionMoveCheck(color, type, 'L', tempSquare, x_coord, y_coord, num);
	}
}

/**
	Checks for posible moves in a single specified direction for a given rook piece

	@direction
		The direction in which the rook will move towards (up/down/left/right)
		This function is reused for checking the queen's possible
	@type
		String indicating the type of the chess piece (rook/queen)
**/
void gameManager::rookDirectionMoveCheck(string color, string type, char direction, boardSquare * tempSquare, int x_coord, int y_coord, int num)
{
	//genericPiece * tempPiece;
	// Color of the piece that is occupying a potential square that the rook can be moved to
	string tempColor = "";

	// Up
	while (tempSquare->getDirection(direction) != NULL)
	{
		tempSquare = tempSquare->getDirection(direction);

		int occupiedSquareReached = tempSquareOccupantCheck(color, type, num, tempSquare, x_coord, y_coord);

		// Stop if an occupied square has been reached
		if (occupiedSquareReached == 1)
		{
			break;
		}
	}
}

/**
	Checks for posible moves in all possible directions for a given knight piece

	@color
		Color of the chess piece
	@num
		Number of the chess piece in its respective array (eg. pawns[num])
	@tempSquare
		Pointer to the boardSquare that the piece currently resides in
	@x_coord
		x-coordinate of the chess piece
	@y_coord
		y-coordinate of the chess piece
**/
void gameManager::knightAllDirectionsCheck(string color, int num, boardSquare * tempSquare, int x_coord, int y_coord)
{
	// Upper right
	if (x_coord < 7 && y_coord > 1)
	{
		knightDirectionCheck(color, 'U', 'R', num, tempSquare, x_coord, y_coord);
		tempSquare = &gameBoard[x_coord][y_coord];
	}
	// Upper left
	if (x_coord > 0 && y_coord > 1)
	{
		knightDirectionCheck(color, 'U', 'L', num, tempSquare, x_coord, y_coord);
		tempSquare = &gameBoard[x_coord][y_coord];
	}
	// Lower right
	if (x_coord < 7 && y_coord < 6)
	{
		knightDirectionCheck(color, 'D', 'R', num, tempSquare, x_coord, y_coord);
		tempSquare = &gameBoard[x_coord][y_coord];
	}
	// Lower left
	if (x_coord > 0 && y_coord < 6)
	{
		knightDirectionCheck(color, 'D', 'L', num, tempSquare, x_coord, y_coord);
		tempSquare = &gameBoard[x_coord][y_coord];
	}
	// Right up (different from upper right)
	if (x_coord < 6 && y_coord > 0)
	{
		knightDirectionCheck(color, 'R', 'U', num, tempSquare, x_coord, y_coord);
		tempSquare = &gameBoard[x_coord][y_coord];
	}
	// Right down
	if (x_coord < 6 && y_coord < 7)
	{
		knightDirectionCheck(color, 'R', 'D', num, tempSquare, x_coord, y_coord);
		tempSquare = &gameBoard[x_coord][y_coord];
	}
	// Left up
	if (x_coord > 1 && y_coord > 0)
	{
		knightDirectionCheck(color, 'L', 'U', num, tempSquare, x_coord, y_coord);
		tempSquare = &gameBoard[x_coord][y_coord];
	}
	// Left down
	if (x_coord > 1 && y_coord < 7)
	{
		knightDirectionCheck(color, 'L', 'D', num, tempSquare, x_coord, y_coord);
	}
}

/**
	Checks for posible moves in a specific direction for a given knight piece

	@dir1
		The direction of the long end of the L-shaped move of the knight
	@dir2
		The direction of the short end of the L-shaped move of the knight
**/
void gameManager::knightDirectionCheck(string color, char dir1, char dir2, int num, boardSquare * tempSquare, int x_coord, int y_coord)
{
	tempSquare = tempSquare->getDirection(dir1);
	tempSquare = tempSquare->getDirection(dir1);
	tempSquare = tempSquare->getDirection(dir2);

	tempSquareOccupantCheck(color, "knight", num, tempSquare, x_coord, y_coord);
}

/**
	Checks for posible moves in all possible directions for a given bishop piece
	This function is reused for checking the queen's possible moves

	@type
		String indicating the type of the chess piece (bishop/queen)
**/
void gameManager::bishopAllDirectionCheck(string color, string type, boardSquare * tempSquare, int x_coord, int y_coord, int num)
{
	// Upper right
	if (x_coord < 7 && y_coord > 0)
	{
		bishopDirectionMoveCheck(color, type, 'R', 'U', tempSquare, x_coord, y_coord, num);
	}
	// Upper left
	if (x_coord > 0 && y_coord > 0)
	{
		bishopDirectionMoveCheck(color, type, 'L', 'U', tempSquare, x_coord, y_coord, num);
	}
	// Lower right
	if (x_coord < 7 && y_coord < 7)
	{
		bishopDirectionMoveCheck(color, type, 'R', 'D', tempSquare, x_coord, y_coord, num);
	}
	// Lower left
	if (x_coord > 0 && y_coord < 7)
	{
		bishopDirectionMoveCheck(color, type, 'L', 'D', tempSquare, x_coord, y_coord, num);
	}
}

/**
	Checks for posible moves in a single specified direction for a given bishop piece

	@direction
		The horizontal direction that the bishop will move towards (hor + vert = diagonal)
	@vertDirection
		The vertical direction that the bishop will move towards
	@type
		String indicating the type of the chess piece (bishop/queen)
**/
void gameManager::bishopDirectionMoveCheck(string color, string type, char direction, char vertDirection, boardSquare * tempSquare, int x_coord, int y_coord, int num)
{
	//genericPiece * tempPiece;
	// Color of the piece that is occupying a potential square that the rook can be moved to
	string tempColor = "";
	pair<int, int> toAdd;

	while (tempSquare->getDirection(direction) != NULL && tempSquare->getDirection(vertDirection) != NULL)
	{
		tempSquare = tempSquare->getDirection(direction);
		tempSquare = tempSquare->getDirection(vertDirection);

		int occupiedSquareReached = tempSquareOccupantCheck(color, type, num, tempSquare, x_coord, y_coord);

		// Stop if an occupied square has been reached
		if (occupiedSquareReached == 1)
		{
			break;
		}
	}
}

/**
	Checks for posible moves in all possible directions for a given king piece
**/
void gameManager::kingAllDirectionsCheck(string color, int num, boardSquare * tempSquare, int x_coord, int y_coord)
{
	// Up
	if (y_coord > 0)
	{
		tempSquare = tempSquare->getDirection('U');
		tempSquareOccupantCheck(color, "king", num, tempSquare, x_coord, y_coord);
		// Reset tempSquare to the king's position
		tempSquare = &gameBoard[x_coord][y_coord];
	}
	// Down
	if (y_coord < 7)
	{
		tempSquare = tempSquare->getDirection('D');
		tempSquareOccupantCheck(color, "king", num, tempSquare, x_coord, y_coord);
		tempSquare = &gameBoard[x_coord][y_coord];
	}
	// Left
	if (x_coord > 0)
	{
		tempSquare = tempSquare->getDirection('L');
		tempSquareOccupantCheck(color, "king", num, tempSquare, x_coord, y_coord);
		tempSquare = &gameBoard[x_coord][y_coord];
	}
	// Right
	if (x_coord < 7)
	{
		tempSquare = tempSquare->getDirection('R');
		tempSquareOccupantCheck(color, "king", num, tempSquare, x_coord, y_coord);
		tempSquare = &gameBoard[x_coord][y_coord];
	}

	// Upper right
	if (x_coord < 7 && y_coord > 0)
	{
		tempSquare = tempSquare->getDirection('U');
		tempSquare = tempSquare->getDirection('R');

		tempSquareOccupantCheck(color, "king", num, tempSquare, x_coord, y_coord);
		tempSquare = &gameBoard[x_coord][y_coord];
	}
	// Upper left
	if (x_coord > 0 && y_coord > 0)
	{
		tempSquare = tempSquare->getDirection('U');
		tempSquare = tempSquare->getDirection('L');

		tempSquareOccupantCheck(color, "king", num, tempSquare, x_coord, y_coord);
		tempSquare = &gameBoard[x_coord][y_coord];
	}
	// Lower right
	if (x_coord < 7 && y_coord < 7)
	{
		tempSquare = tempSquare->getDirection('D');
		tempSquare = tempSquare->getDirection('R');

		tempSquareOccupantCheck(color, "king", num, tempSquare, x_coord, y_coord);
		tempSquare = &gameBoard[x_coord][y_coord];
	}
	// Lower left
	if (x_coord > 0 && y_coord < 7)
	{
		tempSquare = tempSquare->getDirection('D');
		tempSquare = tempSquare->getDirection('L');

		tempSquareOccupantCheck(color, "king", num, tempSquare, x_coord, y_coord);
		tempSquare = &gameBoard[x_coord][y_coord];
	}
}

/******** 
	Helper functions related to making a move 
********/

/**
	Prompts the current player for a move
	Will continue prompting until a valid move is given
	Knights are referred to by using "h" for horse instead of k, for easier parsing (knights vs kings)

	@player
		The color of the current player (black/white)
**/
void gameManager::movePrompt(string player)
{
	movedFrom = NULL;
	movedTo = NULL;
	movedFromX = -1, movedFromY = -1, movedToX = -1, movedToY = -1;

	movedFromOrig = NULL;
	movedToOrig = NULL;
	movedFromOrigX = -1, movedFromOrigY = -1, movedToOrigX = -1, movedToOrigY = -1;

	bool validMove = false;
	string input = "", front = "", back = "", color = "", type = "", numStr = "";
	int num = -1, x_coord = -1, y_coord = -1;

	while (!validMove)
	{
		//drawBoard();
		cout << player << "'s turn" << endl;

		moved = false;

		cout << "Please enter a valid move: ";
		cin >> input;
		
		string delimiter = ">";
		// Separate the front and back strings
		front = input.substr(0, input.find(delimiter));
		back = input.substr(input.find(delimiter) + 1, input.size() - 1);

		/***** START: Input validity checks *****/

		bool inputIsValid = inputValidityCheck(input, front, back, player, color, type, numStr, num, x_coord, y_coord);

		if (!inputIsValid)
		{
			continue;
		}

		/***** END: Input validity checks *****/

		pair<int, int> toCheck = make_pair(x_coord, y_coord);
		vector< pair<int, int> > moveList;
		//bool isValidMove = false;

		// Pawns
		if (type.compare("p") == 0)
		{
			if (color.compare("b") == 0)
			{
				num += 8;
			}

			validMove = validMoveCheck("pawn", num, toCheck);
		}
		// Rooks, bishops and knights
		else if (type.compare("r") == 0 || type.compare("b") == 0 || type.compare("h") == 0)
		{
			if (color.compare("b") == 0)
			{
				num += 2;
			}

			// Rook
			if (type.compare("r") == 0)
			{
				validMove = validMoveCheck("rook", num, toCheck);
			}
			// Knight
			else if (type.compare("h") == 0)
			{
				validMove = validMoveCheck("knight", num, toCheck);
			}
			// Bishop
			else if (type.compare("b") == 0)
			{
				validMove = validMoveCheck("bishop", num, toCheck);
			}
		}
		// Queens
		else if (type.compare("q") == 0 || type.compare("k") == 0)
		{
			if (color.compare("w") == 0)
			{
				num = 0;
			}
			else if(color.compare("b") == 0)
			{
				num = 1;
			}

			// Queen
			if (type.compare("q") == 0)
			{
				validMove = validMoveCheck("queen", num, toCheck);
			}
			// King
			else if (type.compare("k") == 0)
			{
				validMove = validMoveCheck("king", num, toCheck);
			}	
		}

		if (!validMove)
		{
			drawBoard();
			cout << "INVALID MOVE" << endl;
			continue;
		}

		// Checkmate check here
		// If the current player makes a move that leads to his own king being checked, it is an illegal move
		// If this happens, undo the move and change validMove to FALSE

		checkPossibleMoves();

		int currentP_checkmateStatus = 0;
		int enemy_checkmateStatus = 0;
		string enemy = "";

		currentP_checkmateStatus = checkMateStatus(player);
		if (currentP_checkmateStatus == 1 || currentP_checkmateStatus == 2)
		{
			validMove = false;
			undoLastMove(true);

			drawBoard();
			cout << "INVALID MOVE, MOVING INTO CHECK" << endl;
			continue;
		}

		if (player.compare("w") == 0)
		{
			enemy = "b";
		}
		else
		{
			enemy = "w";
		}

		enemy_checkmateStatus = checkMateStatus(enemy);
		if (enemy_checkmateStatus == 1)
		{
			drawBoard();
			cout << enemy << " KING HAS BEEN CHECKED" << endl;
		}
		else if (enemy_checkmateStatus == 2)
		{
			drawBoard();
			cout << enemy << " KING HAS BEEN CHECKMATED." << endl;
			cout << player << " WINS!" << endl;
			gameOver = true;
		}
		else
		{
			drawBoard();
		}
	}
}

/**
	Verifies the validity of the input that is given by a player.
	Returns FALSE if the input is invalid, TRUE otherwise.

	@input
		The entire input string that is given by the player
	@front
		The front portion of the input string (front and back are separated by a delimiter ">")
	@back
		The back portion of the input string
	@player
		The current player's turn (black/white)
	@&color
		The color of the piece that is to be moved
	@&type
		The type of the piece that is to be moved
	@&numstr
		The string containing the number of the chesspiece that is to be moved (without adjustments applied to it yet)
	@&num
		numStr after it has been converted to an int
	@&x_coord
		The x-coordinate that the chess piece will be moved to
	@&y_coord
		The y-coordinate that the chess piece will be moved to
**/
bool gameManager::inputValidityCheck(string input, string front, string back, string player, string &color, string &type, string &numStr, int &num, int &x_coord, int &y_coord)
{
	/** Front portion **/
	if (front.size() > 3 || front.size() < 3 || back.size() < 2)
	{
		cout << "INVALID COMMAND" << endl;
		return false;
	}

	// Get the color, type and num from the front string
	color = input.substr(0, 1);
	type = input.substr(1, 1);
	numStr = input.substr(2, 1);

	// Color checker
	if (color.compare("w") != 0 && color.compare("b") != 0)
	{
		cout << "INVALID COLOR" << endl;
		return false;
	}
	// Player turn checker
	if (player.compare(color) != 0)
	{
		cout << "INVALID PLAYER MOVE. CURRENT TURN: " << player << endl;
		return false;
	}
	// Type checker
	if (type.compare("p") != 0 && type.compare("r") != 0 && type.compare("h") != 0 && type.compare("k") != 0 && type.compare("b") != 0 && type.compare("q") != 0)
	{
		cout << "INVALID TYPE" << endl;
		return false;
	}
	// Only pawns are allowed to go past 0 and 1, cont'd below
	if (type.compare("p") != 0 && numStr.compare("0") != 0 && numStr.compare("1") != 0)
	{
		cout << "INVALID CHESS PIECE NUMBER" << endl;
		return false;
	}
	// Up to a maximum of 7 (8 pawns)
	if (type.compare("p") == 0 && numStr.compare("0") != 0 && numStr.compare("1") != 0 && numStr.compare("2") != 0 && numStr.compare("3") != 0
		&& numStr.compare("4") != 0 && numStr.compare("5") != 0 && numStr.compare("6") != 0 && numStr.compare("7") != 0)
	{
		cout << "INVALID CHESS PIECE NUMBER" << endl;
		return false;
	}
	num = stoi(numStr);

	/** Back portion **/
	if (back.compare("possiblemoves") == 0)
	{
		// TODO make this happen
		
	}
	else
	{
		string x_str = back.substr(0, 1);
		string y_str = back.substr(1, 1);

		// Checks to see if the x and y coordinates given are numbers within the expected range
		if (x_str.compare("0") != 0 && x_str.compare("1") != 0 && x_str.compare("2") != 0 && x_str.compare("3") != 0 && x_str.compare("4") != 0
			&& x_str.compare("5") != 0 && x_str.compare("6") != 0 && x_str.compare("7"))
		{
			cout << "INVALID X COORDINATE" << endl;
			return false;
		}
		if (y_str.compare("0") != 0 && y_str.compare("1") != 0 && y_str.compare("2") != 0 && y_str.compare("3") != 0 && y_str.compare("4") != 0
			&& y_str.compare("5") != 0 && y_str.compare("6") != 0 && y_str.compare("7"))
		{
			cout << "INVALID Y COORDINATE" << endl;
			return false;
		}

		x_coord = stoi(x_str);
		y_coord = stoi(y_str);
	}

	return true;
}

/**
	Check to see if the given piece can move to the coordinates given in toCheck. <<<<<<<<<< THIS SEEMS REDUNDANT
	Returns TRUE if the specified move is valid, FALSE otherwise.
**/
bool gameManager::validMoveCheck(string type, int num, pair<int, int> toCheck)
{
	vector< pair<int, int> > moveList;
	boardSquare movedFromSquare;

	if (type.compare("pawn") == 0)
	{
		return movePiece("pawn", num, toCheck, true);
	}
	else if (type.compare("rook") == 0)
	{
		return movePiece("rook", num, toCheck, true);
	}
	else if (type.compare("knight") == 0)
	{
		return movePiece("knight", num, toCheck, true);
	}
	else if (type.compare("bishop") == 0)
	{
		return movePiece("bishop", num, toCheck, true);
	}
	else if (type.compare("king") == 0)
	{
		return movePiece("king", num, toCheck, true);
	}
	else if (type.compare("queen") == 0)
	{
		return movePiece("queen", num, toCheck, true);
	}

	return true;
}

/**
	Moves the specified piece to the coordinates provided in toCheck.
	Returns TRUE if the move is possible, FALSE otherwise.

	@type
		The type of the piece to be moved
	@num
		Number of the chess piece in its respective array (eg. pawns[num])
	@toCheck
		The x and y coordinates of the square that the piece will be moved to
	@recordOrigInfo
		Boolean used to indicate whether extra info about the move should be recorded (for undoing a move in the movePrompt function)
**/
bool gameManager::movePiece(string type, int num, pair<int, int> toCheck, bool recordOrigInfo)
{
	vector< pair<int, int> > moveList;
	boardSquare movedFromSquare;


	if (type.compare("pawn") == 0)
	{
		moveList = pawns[num].getPossibleMoves();
		movedFrom = &pawns[num];
	}
	else if (type.compare("rook") == 0)
	{
		moveList = rooks[num].getPossibleMoves();
		movedFrom = &rooks[num];
	}
	else if(type.compare("knight") == 0)
	{
		moveList = knights[num].getPossibleMoves();
		movedFrom = &knights[num];
	}
	else if (type.compare("bishop") == 0)
	{
		moveList = bishops[num].getPossibleMoves();
		movedFrom = &bishops[num];
	}
	else if (type.compare("queen") == 0)
	{
		moveList = queens[num].getPossibleMoves();
		movedFrom = &queens[num];
	}
	else if (type.compare("king") == 0)
	{
		moveList = kings[num].getPossibleMoves();
		movedFrom = &kings[num];
	}
	
	// Check to see if the coordinates given in toCheck are a valid move location for the chess piece
	// If valid, then move the piece
	if (find(moveList.begin(), moveList.end(), toCheck) != moveList.end())
	{
		// Save the original x and y coordinates of the piece to be moved (for the undo function)
		movedFromX = movedFrom->getXCoord();
		movedFromY = movedFrom->getYCoord();

		// Check to see if the square that we want to move to is already occupied
		// If it is, kill the piece in it (enemy chess piece is guaranteed)
		movedToX = toCheck.first;
		movedToY = toCheck.second;
		if (gameBoard[movedToX][movedToY].isOccupied())
		{
			movedTo = gameBoard[movedToX][movedToY].getOccupant();
			movedTo->kill();
		}
		else
		{
			movedTo = NULL;
		}
		
		// Record "original" move info for undoing later on
		if (recordOrigInfo)
		{
			movedFromOrigX = movedFrom->getXCoord();
			movedFromOrigY = movedFrom->getYCoord();

			string tempMovedFromColor = movedFrom->getColor();
			string tempMovedFromType = movedFrom->getType();
			int tempMovedFromNum = adjustedNum(tempMovedFromColor, tempMovedFromType, movedFrom->getNum());

			// Make the movedFromOrig pointer point directly to the chess piece
			if (tempMovedFromType.compare("pawn") == 0)
			{
				movedFromOrig = &pawns[tempMovedFromNum];
			}
			else if (tempMovedFromType.compare("rook") == 0)
			{
				movedFromOrig = &rooks[tempMovedFromNum];
			}
			else if (tempMovedFromType.compare("knight") == 0)
			{
				movedFromOrig = &knights[tempMovedFromNum];
			}
			else if (tempMovedFromType.compare("bishop") == 0)
			{
				movedFromOrig = &bishops[tempMovedFromNum];
			}
			else if (tempMovedFromType.compare("king") == 0)
			{
				movedFromOrig = &kings[tempMovedFromNum];
			}
			else if (tempMovedFromType.compare("queen") == 0)
			{
				movedFromOrig = &queens[tempMovedFromNum];
			}

			movedToOrigX = toCheck.first;
			movedToOrigY = toCheck.second;
			if (gameBoard[movedToOrigX][movedToOrigY].isOccupied())
			{
				string tempMovedToColor = movedTo->getColor();
				string tempMovedToType = movedTo->getType();
				int tempMovedToNum = adjustedNum(tempMovedToColor, tempMovedToType, movedTo->getNum());

				// Make the movedToOrig pointer point directly to the chess piece
				if (tempMovedToType.compare("pawn") == 0)
				{
					movedToOrig = &pawns[tempMovedToNum];
				}
				else if (tempMovedToType.compare("rook") == 0)
				{
					movedToOrig = &rooks[tempMovedToNum];
				}
				else if (tempMovedToType.compare("knight") == 0)
				{
					movedToOrig = &knights[tempMovedToNum];
				}
				else if (tempMovedToType.compare("bishop") == 0)
				{
					movedToOrig = &bishops[tempMovedToNum];
				}
				else if (tempMovedToType.compare("king") == 0)
				{
					movedToOrig = &kings[tempMovedToNum];
				}
				else if (tempMovedToType.compare("queen") == 0)
				{
					movedToOrig = &queens[tempMovedToNum];
				}
			}
			else
			{
				movedToOrig = NULL;
			}
		}

		// Set the new x+y coordinates for the moved piece
		movedFrom->setXY(movedToX, movedToY);
		// Update moved pieces' location on the board
		gameBoard[movedFromX][movedFromY].removeOccupant();
		gameBoard[movedToX][movedToY].setOccupant(movedFrom);

		moved = true;
	}
	else
	{
		return false;
	}

	return true;
}

/**
	Undoes the last move made.
	If undoOrig is true, the move that is undone is the special one that is recorded in movePiece

	@undoOrig
		Boolean that indicates whether a normal undo move or a special "original" undo move should be done
**/
void gameManager::undoLastMove(bool undoOrig)
{
	if (!undoOrig)
	{
		// If a piece was captured
		if (movedTo != NULL)
		{
			// Revive the captured piece and restore it to its original position
			movedTo->revive();
			gameBoard[movedToX][movedToY].setOccupant(movedTo);
		}
		// Nothing captured
		else
		{
			// Remove the occupant of the square that was moved to
			// Nothing was captured, so the square should be empty when the move is undone
			gameBoard[movedToX][movedToY].removeOccupant();
		}

		// Move the piece pointed to by movedFrom back to its original location
		movedFrom->setXY(movedFromX, movedFromY);
		gameBoard[movedFromX][movedFromY].setOccupant(movedFrom);
		checkPossibleMoves();
	}
	// Undo the "original" move
	else
	{
		// If a piece was captured
		if (movedToOrig != NULL)
		{
			// Revive the captured piece and restore it to its original position
			movedToOrig->revive();
			gameBoard[movedToOrigX][movedToOrigY].setOccupant(movedToOrig);
		}
		// Nothing captured
		else
		{
			// Remove the occupant of the square that was moved to
			// Nothing was captured, so the square should be empty when the move is undone
			gameBoard[movedToOrigX][movedToOrigY].removeOccupant();
		}

		// Move the piece pointed to by movedFrom back to its original location
		movedFromOrig->setXY(movedFromOrigX, movedFromOrigY);
		gameBoard[movedFromOrigX][movedFromOrigY].setOccupant(movedFromOrig);
		checkPossibleMoves();
	}
}

/**
	Given a color, chess piece type and its number on the board, returns a number that corresponds to the chess piece's position in its respective array.
	(eg. black pawn 0 returns a num value of 8 - b_p_0 is located at pawns[8])
**/
int gameManager::adjustedNum(string color, string type, int num)
{
	int toReturn = num;
	if (color.compare("b") == 0)
	{
		if (type.compare("pawn") == 0)
		{
			toReturn += 8;
		}
		else if (type.compare("rook") == 0 || type.compare("knight") == 0 || type.compare("bishop") == 0)
		{
			toReturn += 2;
		}
		else if (type.compare("king") == 0 || type.compare("queen") == 0)
		{
			toReturn = 1;
		}
	}

	return toReturn;
}

/********** 
	Helper functions related to printing the board state on the console 
**********/

/**
	Prints the current state of the chess board on the console.
**/
void gameManager::drawBoard()
{
	// Print x-axis grid markings
	for (int i = 0; i < 8; i++)
	{
		cout << "   (" << i << ")  ";
	}
	cout << endl;

	// Print top border (Don't repeat/loop this)
	printBorder();

	// Loop the rest
	for (int y = 0; y < 8; y++)
	{
		// Print 1st line
		printBlankLine();

		// Print 2nd line
		for (int x = 0; x < 8; x++)
		{
			cout << "|";

			if (gameBoard[x][y].isOccupied())
			{
				genericPiece * tempPiece = gameBoard[x][y].getOccupant();
				string type = tempPiece->getType();

				if (type.compare("pawn") == 0)
				{
					cout << " " << tempPiece->getColor() << "_p_" << tempPiece->getNum() << " ";
				}
				else if (type.compare("rook") == 0)
				{
					cout << " " << tempPiece->getColor() << "_r_" << tempPiece->getNum() << " ";
				}
				else if (type.compare("knight") == 0)
				{
					cout << " " << tempPiece->getColor() << "_h_" << tempPiece->getNum() << " ";
				}
				else if (type.compare("bishop") == 0)
				{
					cout << " " << tempPiece->getColor() << "_b_" << tempPiece->getNum() << " ";
				}
				else if (type.compare("king") == 0)
				{
					cout << " " << tempPiece->getColor() << "_kng" << " ";
				}
				else if (type.compare("queen") == 0)
				{
					cout << " " << tempPiece->getColor() << "_que" << " ";
				}
				else cout << " _def_ ";
			}
			else
			{
				cout << "       ";
			}
		}
		cout << "|";

		// Print y-axis grid markings
		cout << " (" << y << ")" << endl;

		// Print 3rd line
		printBlankLine();

		// Print bottom border
		printBorder();
	}
}

/**
	'Clears' whatever is currently displayed on the console
**/
void gameManager::clearScreen()
{
	int n;
	for (n = 0; n < 10; n++)
		printf("\n\n\n\n\n\n\n\n\n\n");
}

/**
	Prints the top/bottom border of the board
**/
void gameManager::printBorder()
{
	for (int i = 0; i < 65; i++)
	{
		cout << "*";
	}
	cout << endl;
}

/**
	Prints a blank line on the board
**/
void gameManager::printBlankLine()
{
	for (int i = 0; i < 8; i++)
	{
		cout << "|";
		cout << "       ";
	}
	cout << "|" << endl;
}