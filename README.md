# Chess
Command-line based chess game

#README

---------------
Summary
---------------
· Command-line based chess game.
· Written in C++ using Visual Studio 2015.
· Fully functional two-player chess game, with accurate moves and checkmate checks.
· Uses several classes and data structures to represent the board state and chess pieces, in accordance
with Object-oriented programming philosophy.
· On the fully traversable game board, chess piece moves are determined according to their respective
movesets.

---------------
Details
---------------
· The project contains a total of 3 classes: boardSquare, genericPiece and gameManager.

· genericPiece is a class containing the data structure boardSquare, which is used to represent a chess piece.
· genericPiece is a template which is extended from to make the respective chess piece types (Pawn/Rook/Knight/Bishop/King/Queen)
· The boardSquare data structure contains the following info:
  - The x and y coordinate of the chess piece
  - A vector of pairs of integers (pair<int, int>), which denotes the possible moves that the chess piece can take. I used vectors for easy resizing.
  - A boolean that tells if the chess piece is dead or not
  - Strings denoting the color and type of the chess piece
  - An integer denoting the number of the chess piece in its respective array (more info about this below)

· boardSquare is a class containing the data structure boardSquare, which is used to represent a single square on the board.
· The boardSquare data structure contains the following info:
  - The x and y coordinate of the square
  - A pointer to the chess piece that is currently occupying the square (if available)
  - A boolean that tells if the square is occupied or not
  - Pointers to 4 other boardSquares, each representing a direction, Up/Down/Left/Right
· A boardSquare is kind of a 4 way linked list of sorts. When the board is completely linked, all 64 squares are fully traversable from edge to edge.

· gameManager is the class that is used to run the chess game. It contains a method called run(), that when called, will run the chess game from start to finish.
· gameManager contains several helper methods which are used to help initialize and run the chess game.

---------------
Implementation
---------------
· The board is represented by a 8x8 array of boardSquares, which are all linked at the start of the game.
· The x and y coordinates go from 0-7 respectively.
· Each type of chess piece has its own array, with the array containing all possible chess pieces of that type.
· The first half of the array contains white's pieces, and the 2nd half contains black's pieces.
· Eg. the pawns array has size 16, pawns[0-7] are white's pawns, and pawns[8-15] are black's pawns.
· Chess piece moves are found by calling a helper function unique to each chess piece type, which then finds all possible moves for a given chess piece at coordinates (x, y).
· Checkmate checks are done by first determining if a king of a given color is being threatened.
· If so, then we iterate through all possible moves of that given color. If any one of the possible moves results in the king being safe, then it is not a checkmate.
· A player is not allowed to make a move that will result in his/her king being threatened.

· The board is represented on the console as follows:
· x and y coordinates go from 0-7

*****************************************************************
|       |       |       |       |       |       |       |       |
| b_r_0 | b_h_0 | b_b_0 | b_que | b_kng | b_b_1 | b_h_1 | b_r_1 |
|       |       |       |       |       |       |       |       |
*****************************************************************
|       |       |       |       |       |       |       |       |
| b_p_0 | b_p_1 | b_p_2 | b_p_3 | b_p_4 | b_p_5 | b_p_6 | b_p_7 |
|       |       |       |       |       |       |       |       |
*****************************************************************
|       |       |       |       |       |       |       |       |
|       |       |       |       |       |       |       |       |
|       |       |       |       |       |       |       |       |
*****************************************************************
|       |       |       |       |       |       |       |       |
|       |       |       |       |       |       |       |       |
|       |       |       |       |       |       |       |       |
*****************************************************************
|       |       |       |       |       |       |       |       |
|       |       |       |       |       |       |       |       |
|       |       |       |       |       |       |       |       |
*****************************************************************
|       |       |       |       |       |       |       |       |
|       |       |       |       |       |       |       |       |
|       |       |       |       |       |       |       |       |
*****************************************************************
|       |       |       |       |       |       |       |       |
| w_p_0 | w_p_1 | w_p_2 | w_p_3 | w_p_4 | w_p_5 | w_p_6 | w_p_7 |
|       |       |       |       |       |       |       |       |
*****************************************************************
|       |       |       |       |       |       |       |       |
| w_r_0 | w_h_0 | w_b_0 | w_que | w_kng | w_b_1 | w_h_1 | w_r_1 |
|       |       |       |       |       |       |       |       |
*****************************************************************

· The first letter represents the color of the chess piece.
· The second letter represents the type of the chess piece.
· The third letter represents the number of the chess piece.
· Knights are represented as h for easier parsing.
· A move command is given as follows. Eg. wh1>65
· To move the king and queen, the move command is as follows:
  Eg. [w/b][k/q][0/1]>[x coordinate][y coordinate]
· The numbers behind k/q don't matter (can be either 0 or 1), as they are there only for standardizing the input
· If an invalid move command is made, the player is prompted for another move until a valid move is given.
· The game ends when gameManager detects that either player has been checkmated.

NOTE:
· The game is currently missing promotion, castling and en passant, which will be added later.
· The game is fully functional otherwise.
