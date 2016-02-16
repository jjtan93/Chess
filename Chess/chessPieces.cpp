#include "chessPieces.h"

using namespace std;

// Constructor
genericPiece::genericPiece()
{
	type = "NONE";
	color = "NONE";
	x_coordinate = -1;
	y_coordinate = -1;

	// Used vector instead of array (possible moves) for memory management purposes (easy resizing)
	// Initialized to [64][2], 64 because there are 64 squares on the board (lazy solution)
	// Each entry possibleMoves[i][0] and [i][1] represent a possible move location for the piece
	// [i][0] is the x-coordinate and [i][1] is the y-coordinate for each possible move
	possibleMoves.resize(0);// inside supposed to be 2 not 4

	dead = false;
}

// Destructor (Unused)
genericPiece::~genericPiece()
{
	
}

// Returns the color of the chesspiece
string genericPiece::getColor()
{
	return color;
}

// Returns the type of the chesspiece, eg. queen / knight/ pawn, etc...
string genericPiece::getType()
{
	return type;
}

void genericPiece::setXY(int x, int y)
{
	x_coordinate = x;
	y_coordinate = y;
}

// Returns the current x-coordinate of the chesspiece
int genericPiece::getXCoord()
{
	return x_coordinate;
}

// Returns the current y-coordinate of the chesspiece
int genericPiece::getYCoord()
{
	return y_coordinate;
}

// Returns a vector of all possible moves for the chesspiece
vector< pair<int, int> > genericPiece::getPossibleMoves()
{
	return possibleMoves;
}

// Sets the possible moves for the chesspiece
// The checking of possible moves is done by the gameManager
void genericPiece::addPossibleMove(pair<int, int> toAdd)
{
	//possibleMoves = movesToSet;
	possibleMoves.push_back(toAdd);
}

void genericPiece::clearPossibleMoves()
{
	possibleMoves.resize(0);
}

// Changes the status of the chesspiece to 'DEAD'
void genericPiece::kill()
{
	dead = true;
}

void genericPiece::revive()
{
	dead = false;
}

// Returns the status of the chesspiece (dead/alive)
bool genericPiece::isDead()
{
	return dead;
}

void genericPiece::setNum(int toSet)
{
	num = toSet;
}

int genericPiece::getNum()
{
	return num;
}

void genericPiece::setColor(string toSet)
{
	color = toSet;
}

/** Self explanatory **/
pawn::pawn()
{
	type = "pawn";
}

rook::rook()
{
	type = "rook";
}

knight::knight()
{
	type = "knight";
}

bishop::bishop()
{
	type = "bishop";
}

king::king()
{
	type = "king";
}

queen::queen()
{
	type = "queen";
}
