#include <string>
#include <vector>

using namespace std;

// Generic class that is extended from to create new chesspieces.
class genericPiece
{
public:
	genericPiece();
	~genericPiece();
	
	void setColor(string toSet);
	void setNum(int toSet);
	void setXY(int x, int y);
	void addPossibleMove(pair<int, int> toAdd);
	void clearPossibleMoves();

	string getColor();
	string getType();
	int getNum();
	int getXCoord();
	int getYCoord();
	vector< pair<int, int> > getPossibleMoves();
	
	void kill();
	void revive();
	bool isDead();
	
protected:
	string type;
	string color;

	int num;
	int count;
	
	int x_coordinate;
	int y_coordinate;
	vector< pair<int, int> > possibleMoves;
	bool dead;
};

class pawn : public genericPiece
{
public:
	pawn();

protected:
};

class rook : public genericPiece
{
public:
	rook();

protected:
};

class knight : public genericPiece
{
public:
	knight();

protected:
};

class bishop : public genericPiece
{
public:
	bishop();

protected:
};

class king : public genericPiece
{
public:
	king();

protected:
};

class queen : public genericPiece
{
public:
	queen();

protected:
};