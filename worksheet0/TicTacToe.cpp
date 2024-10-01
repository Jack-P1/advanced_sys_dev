#include <iostream>
#include "TicTacToe.hpp"

void TicTacToe::clearGrid()
{
	for (unsigned int i = 0; i < 9; i++) {
		grid[i] = 3;
	}
}

bool TicTacToe::checkValidMove(const unsigned int x, bool player1) 
{
	if (player1) {
		if (grid[x] == 2 || x > 8) {
			return false;
		}
		else {
			return true;
		}
	}
	else {
		if (grid[x] == 1 || x > 8) {
			return false;
		}
		else {
			return true;
		}
	}

}

bool TicTacToe::checkIfWin(const unsigned int pos) 
{
	switch (pos) {
	case(0):
		if (grid[1] == grid[0] && grid[2] == grid[0]) {
			return true;
		}
		else if (grid[3] == grid[0] && grid[6] == grid[0]) {
			return true;
		}
		else if (grid[4] == grid[0] && grid[8] == grid[0]) {
			return true;
		}
		else {
			return false;
		}
		break;
	case(1):
		if (grid[0] == grid[1] && grid[2] == grid[1]) {
			return true;
		}
		else if (grid[4] == grid[1] && grid[7] == grid[1]) {
			return true;
		}
		else {
			return false;
		}
		break;
	case(2):
		if (grid[1] == grid[2] && grid[0] == grid[2]) {
			return true;
		}
		else if (grid[5] == grid[2] && grid[8] == grid[2]) {
			return true;
		}
		else if (grid[4] == grid[2] && grid[6] == grid[2]) {
			return true;
		}
		else {
			return false;
		}
		break;
	case(3):
		if (grid[0] == grid[3] && grid[6] == grid[3]) {
			return true;
		}
		else if (grid[4] == grid[3] && grid[5] == grid[3]) {
			return true;
		}
		else {
			return false;
		}
		break;
	case(4):
		if (grid[0] == grid[4] && grid[8] == grid[4]) {
			return true;
		}
		else if (grid[1] == grid[4] && grid[7] == grid[4]) {
			return true;
		}
		else if (grid[3] == grid[4] && grid[5] == grid[4]) {
			return true;
		}
		else if (grid[2] == grid[4] && grid[6] == grid[4]) {
			return true;
		}
		else {
			return false;
		}
		break;
	case(5):
		if (grid[2] == grid[5] && grid[8] == grid[5]) {
			return true;
		}
		else if (grid[3] == grid[5] && grid[4] == grid[5]) {
			return true;
		}
		else {
			return false;
		}
		break;
	case(6):
		if (grid[7] == grid[6] && grid[8] == grid[6]) {
			return true;
		}
		else if (grid[3] == grid[6] && grid[0] == grid[6]) {
			return true;
		}
		else if (grid[4] == grid[6] && grid[2] == grid[6]) {
			return true;
		}
		else {
			return false;
		}
		break;
	case(7):
		if (grid[6] == grid[7] && grid[8] == grid[7]) {
			return true;
		}
		else if (grid[4] == grid[7] && grid[1] == grid[7]) {
			return true;
		}
		else {
			return false;
		}
		break;
	case(8):
		if (grid[7] == grid[8] && grid[6] == grid[8]) {
			return true;
		}
		else if (grid[4] == grid[8] && grid[0] == grid[8]) {
			return true;
		}
		else if (grid[5] == grid[8] && grid[2] == grid[8]) {
			return true;
		}
		else {
			return false;
		}
		break;
	default:
		return false;
	}
}

void TicTacToe::play() 
{
	clearGrid();

	std::cout << "Player 1 plays as X, player 2 plays as 0" << std::endl;
	
	bool winFound = false;
	bool player1Turn = false;
	bool player2Turn = false;
	int turns = 0;

	while (!winFound) {
		if (player1Turn) {
			if (turns >= 9 && winFound == false) {
				std::cout << "It's a draw!" << std::endl;
				break;
			}
			std::cout << "Player 1, enter the grid position to play [0-8]: " << std::endl;

			unsigned int input{};

			std::cin >> input;

			if (checkValidMove(input, true)) {
				grid[input] = 1;
			}
			else {
				std::cout << "Invalid move!!" << std::endl;
				continue;
			}

			if (checkIfWin(input)) {
				std::cout << "Player 1 wins!";
				winFound = true;
			}

			player1Turn = false;
			player2Turn = true;
			turns++;

		}
		else if (player2Turn) {
			if (turns >= 9 && winFound == false) {
				std::cout << "It's a draw!" << std::endl;
				break;
			}
			std::cout << "Player 2, enter the grid position to play [0-8]: " << std::endl;

			unsigned int input{};

			std::cin >> input;

			if (checkValidMove(input, false)) {
				grid[input] = 2;
			}
			else {
				std::cout << "Invalid move!!" << std::endl;
				continue;
			}

			if (checkIfWin(input)) {
				std::cout << "Player 2 Wins!";
				winFound = true;
			}

			player2Turn = false;
			player1Turn = true;
			turns++;

		}
		// first turn
		else {
			std::cout << "Player 1, enter the grid position to play [0-8]: " << std::endl;	
			unsigned int input{};

			std::cin >> input;

			if (checkValidMove(input, true)) {
				grid[input] = 1;
				std::cout << grid[input];
			}
			else{
				std::cout << "Invalid move!!" << std::endl;
				continue;
			}

			player2Turn = true;
			turns++;
	
		}
	}
}