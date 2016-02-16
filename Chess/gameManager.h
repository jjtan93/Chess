#include "boardSquare.h"

class gameManager
{
public:
	gameManager();
	~gameManager();
	int run();

	void directionSetterHelper(char direction, int x, int y);
	void setPieceInfoAndLoc(int x, int y, int count, string color, string type);
	
	/** Helper methods related to checking and setting posible moves for chess piece **/
	void checkPossibleMoves();
	void setPossibleMove(string color, string type, int num, int toAddX, int toAddY, int x_coord, int y_coord);
	int tempSquareOccupantCheck(string color, string type, int num, boardSquare * tempSquare, int x_coord, int y_coord);
	void clearAllPossibleMoves();
	int checkMateStatus(string player);
	bool kingIsThreatened(string color, int x_coord, int y_coord);
	bool anyValidMoves(string playerTurn);

	/** Helper methods related to checking possible moves for each of the different chess piece types **/
	void pawnRLUMoveCheck(int x_coord, int y_coord, string color, boardSquare * tempSquare, int num);
	void pawnPossibleDiagonalMoves(string direction, string color, int x, int y, boardSquare * tempSquare, int num);

	void rookDirectionMoveCheck(string color, string type, char direction, boardSquare * tempSquare, int x_coord, int y_coord, int num);
	void rookAllDirectionsCheck(string color, string type, boardSquare * tempSquare, int x_coord, int y_coord, int num);

	void knightDirectionCheck(string color, char dir1, char dir2, int num, boardSquare * tempSquare, int x_coord, int y_coord);
	void knightAllDirectionsCheck(string color, int num, boardSquare * tempSquare, int x_coord, int y_coord);

	void bishopDirectionMoveCheck(string color, string type, char direction, char vertDirection, boardSquare * tempSquare, int x_coord, int y_coord, int num);
	void bishopAllDirectionCheck(string color, string type, boardSquare * tempSquare, int x_coord, int y_coord, int num);

	void kingAllDirectionsCheck(string color, int num, boardSquare * tempSquare, int x_coord, int y_coord);	
	
	/** Helper functions related to making a move **/
	void movePrompt(string player);
	bool inputValidityCheck(string input, string front, string back, string player, string &color, string &type, string &numStr, int &num, int &x_coord, int &y_coord);
	bool validMoveCheck(string type, int num, pair<int, int> toCheck);
	bool movePiece(string type, int num, pair<int, int> toCheck, bool recordOrigInfo);
	void undoLastMove(bool undoOrig);

	int adjustedNum(string color, string type, int num);

	/** Helper functions related to printing the board state on the console **/
	void drawBoard();
	void printBorder();
	void printBlankLine();
	void clearScreen();

protected:
	bool gameOver;
	string turn;
	boardSquare gameBoard[8][8];
	genericPiece* pawns;
	genericPiece* rooks;
	genericPiece* knights;
	genericPiece* bishops;
	genericPiece* kings;
	genericPiece* queens;

	genericPiece * movedFrom;
	genericPiece * movedTo;

	int movedFromX, movedFromY, movedToX, movedToY;

	genericPiece * movedFromOrig;
	genericPiece * movedToOrig;

	int movedFromOrigX, movedFromOrigY, movedToOrigX, movedToOrigY;

	vector< pair<int, int> > whiteMovelist;
	vector< pair<int, int> > blackMovelist;

	vector< pair<int, int> > whiteMovelistSource;
	vector< pair<int, int> > blackMovelistSource;

	vector< pair<int, int> > whitePotentialMovelist;
	vector< pair<int, int> > blackPotentialMovelist;

	bool moved;
};
