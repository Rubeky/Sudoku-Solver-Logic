#pragma once
class SudokuGrid
{
private:
	int gridNumbers[9][9];
	int gridPossible[9][9][9];
public:
	SudokuGrid();
	int returnValue(int row, int col) const;
	bool fillValue(int row, int col, int value);
	bool returnPossible(int row, int col, int value) const;
	bool fillPossible(int row, int col, int value, int isPossible);
	bool isExclusivelyInRow(int col, int boxx, int boxy, int value);
	bool isExclusivelyInCol(int row, int boxx, int boxy, int value);
	bool isOnePossibleInSpot(int row, int col, int value);
	bool isOneInBox(int row, int col, int value);
	bool isOneInRow(int row, int value);
	bool isOneInCol(int col, int value);

	bool isOnePossibleInBox(int row, int col, int value);
	bool isOnePossibleInRow(int row, int setcol, int value);
	bool isOnePossibleInCol(int setrow, int col, int value);

	bool isOnePossible(int row, int col, int value);

	bool isOneNotNear(int row, int col, int value);

	bool isComplete();
	void printSolution();
};

