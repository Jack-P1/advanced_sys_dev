#ifndef TICTACTOE_H
#define TICTACTOE_H
#include <array>

class TicTacToe
{
private:
	std::array<int, 9> grid{};

public:
	bool checkValidMove(const unsigned int x, bool player1);
	bool checkIfWin(const unsigned int pos);
	void clearGrid();
	void play();

};

#endif