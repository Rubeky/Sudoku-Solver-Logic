#include "SudokuGrid.h"
#include <iostream>
#include <string>
using namespace std;

SudokuGrid::SudokuGrid()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			this->gridNumbers[i][j] = -1;
			for (int value = 0; value < 9; value++)
			{
				this->gridPossible[i][j][value] = 1;
			}
		}
	}
}

// Setters and Getters
int SudokuGrid::returnValue(int row, int col) const
{
	return gridNumbers[row][col];
}

bool SudokuGrid::fillValue(int row, int col, int value)
{
	if (value >= 0 && value < 9 && gridNumbers[row][col] == -1) {
		this->gridNumbers[row][col] = value;
		for (int i = 0; i < 9; i++)
		{
			gridPossible[row][col][i] = 0;
		}
		gridPossible[row][col][value] = 1;

		if (gridNumbers[row][col] == value) {
			return 1;
		}
	}
	return 0;
}

bool SudokuGrid::returnPossible(int row, int col, int value) const
{
	return this->gridPossible[row][col][value] == 1;
}

bool SudokuGrid::fillPossible(int row, int col, int value, int isPossible)
{
	this->gridPossible[row][col][value] = isPossible;
	if (gridPossible[row][col][value] == isPossible) {
		return 1;
	}
	return 0;
}


// Basic check functions
bool SudokuGrid::isOneInBox(int row, int col, int value)
{
	int count = 0;
	int rowBase = row / 3;
	int colBase = col / 3;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (this->gridNumbers[i + rowBase * 3][j + colBase * 3] == value)
			{
				count++;
			}
		}
	}

	return count == 1;
}

bool SudokuGrid::isOneInRow(int row, int value)
{
	int count = 0;

	for (int col = 0; col < 9; col++)
	{
		if (this->gridNumbers[row][col] == value)
		{
			count++;
		}
	}

	return count == 1;
}

bool SudokuGrid::isOneInCol(int col, int value)
{
	int count = 0;

	for (int row = 0; row < 9; row++)
	{
		if (this->gridNumbers[row][col] == value)
		{
			count++;
		}
	}

	return count == 1;
}

// Checking pencil marks
bool SudokuGrid::isOnePossibleInBox(int row, int col, int value)
{
	int count = 0;
	int colBase = col / 3;
	int rowBase = row / 3;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (this->gridPossible[i + rowBase * 3][j + colBase * 3][value] == 1)
			{
				count++;
			}
		}
	}
	if (this->gridPossible[row][col][value])
	{
		return count == 1;
	}
	return false;
}

bool SudokuGrid::isOnePossibleInCol(int setrow, int col, int value)
{
	int count = 0;

	for (int row = 0; row < 9; row++)
	{
		if (this->gridPossible[row][col][value] == 1)
		{
			count++;
		}
	}
	if (this->gridPossible[setrow][col][value])
	{
		return count == 1;
	}
	return false;
}

bool SudokuGrid::isOnePossibleInRow(int row, int setcol, int value)
{
	int count = 0;

	for (int col = 0; col < 9; col++)
	{
		if (this->gridPossible[row][col][value] == 1)
		{
			count++;
		}
	}
	if (this->gridPossible[row][setcol][value])
	{
		return count == 1;
	}
	return false;
}

bool SudokuGrid::isOnePossible(int row, int col, int value)
{
	bool one= isOnePossibleInBox(row, col, value);
	bool two = isOnePossibleInCol(row, col, value);
	bool three = isOnePossibleInRow(row, col, value);

	return one || two || three;
}

bool SudokuGrid::isOneNotNear(int row, int col, int value)
{
	bool one = !isOneInBox(row, col, value);
	bool two = !isOneInCol(col, value);
	bool three = !isOneInRow(row, value);

	return one && two && three;
}

// More complex
bool SudokuGrid::isExclusivelyInRow(int col, int boxx, int boxy, int value)
{
	bool out = false;


	for (int i = 0; i < 3; i++)
	{
		if (gridPossible[col][boxx * 3 + i][value])
		{
			out = true;
		}
	}
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			if (row + boxy * 3 != col && gridPossible[row + boxy * 3][col + boxx * 3][value])
			{
				return false;
			}
		}
	}

	return true;
}

bool SudokuGrid::isExclusivelyInCol(int row, int boxx, int boxy, int value)
{
	bool out = false;


	for (int i = 0; i < 3; i++)
	{
		if (gridPossible[boxy * 3 + i][row][value])
		{
			out = true;
		}
	}
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			if (col + boxx * 3 != row && gridPossible[row + boxy * 3][col + boxx * 3][value])
			{
				return false;
			}
		}
	}

	return out;
}

bool SudokuGrid::isOnePossibleInSpot(int row, int col, int value)
{
	int count = 0;
	int position = -1;

	for (int i = 0; i < 9; i++)
	{
		if (this->gridPossible[row][col][i] == 1) {
			count++;
			position = i;
		}
	}
	if (position == value && count == 1)
	{
		return true;
	}
	
	return false;
}

// Checking if sudoku is completely filled
bool SudokuGrid::isComplete()
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			if (gridNumbers[row][col] == -1)
			{
				return false;
			}
		}
	}
	return true;
}

void SudokuGrid::printSolution()
{
	int temp = -2;
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			temp = this->gridNumbers[row][col] + 1;
			cout << temp << " ";
		}
		std::cout << std::endl;
	}
}