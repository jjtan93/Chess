#include "boardSquare.h"

using namespace std;

boardSquare::boardSquare()
{
	occupied = false;
	x_coord = -1;
	y_coord = -1;

}

boardSquare::boardSquare(int x, int y)
{
	occupied = false;
	x_coord = -1;
	y_coord = y;

}

// Returns the x-coordinate of the square
int boardSquare::getSquareXCoord()
{
	return x_coord;
}

// Returns the y-coordinate of the square
int boardSquare::getSquareYCoord()
{
	return y_coord;
}

// Returns the current occupied status of the square (occupied or not)
bool boardSquare::isOccupied()
{
	return occupied;
}

void boardSquare::removeOccupant()
{
	occupied = false;
}

// Returns the current occupant of the square
genericPiece * boardSquare::getOccupant()
{
	return occupant;
}

// Sets the current occupant of the square to whatever is specified
void boardSquare::setOccupant(genericPiece * toSet)
{
	occupant = toSet;
	occupied = true;
}

// Sets the neighbour (up / down / left / right) to whatever is specified
// For the direction parameter:
// U = Up, D = Down, L = Left, R = Right
void boardSquare::setDirection(char direction, boardSquare * toSet)
{
	switch (direction)
	{
	case 'U':
		up = toSet;
		break;
	case 'D':
		down = toSet;
		break;
	case 'L':
		left = toSet;
		break;
	case 'R':
		right = toSet;
		break;
	}
}

void boardSquare::setXY(int x, int y)
{
	x_coord = x;
	y_coord = y;
}

// Returns the specified neighbour
boardSquare * boardSquare::getDirection(char direction)
{
	switch (direction)
	{
	case 'U':
		return up;
		break;
	case 'D':
		return down;
		break;
	case 'L':
		return left;
		break;
	case 'R':
		return right;
		break;
	}
}
