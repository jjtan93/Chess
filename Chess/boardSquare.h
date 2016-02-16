#include <string>
#include "chessPieces.h"

using namespace std;

class boardSquare
{
public:
	boardSquare();
	boardSquare(int x, int y);
	
	void setDirection(char direction, boardSquare * toSet);
	void setXY(int x, int y);
	void setOccupant(genericPiece * toSet);
	void removeOccupant();
	bool isOccupied();

	int getSquareXCoord();
	int getSquareYCoord();
	genericPiece * getOccupant();
	boardSquare * getDirection(char direction);

protected:
	int x_coord;
	int y_coord;
	bool occupied;
	genericPiece * occupant;
	boardSquare * up;
	boardSquare * down;
	boardSquare * left;
	boardSquare * right;
};
