#define WINVER 0x0500

#include <windows.h>
#include <iostream>
#include <sstream>
#include "SudokuGrid.h"
#include <winuser.h>
#include <chrono>
#include <thread>

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

// Global variables
bool isComplete = 0;
bool unsolvable = 0;

// Function declarations
bool initialiseGrid(SudokuGrid* grid);
void checkStraight(SudokuGrid* grid);
void fillPossible(SudokuGrid* grid);
void fillSolution(SudokuGrid* grid);
void LeftClick();
void MouseMove(int x, int y);
void pressNum(int num);
void pressRight();
void pressLeft();
void pressDown();


int main()
{
    // Sets up grid for all values
    SudokuGrid* grid = new SudokuGrid;
    initialiseGrid(grid);

    // Print out initial 
    grid->printSolution();

    // Solving loop
    while (!isComplete && !unsolvable) 
    {
        checkStraight(grid);
        fillPossible(grid);
    }

    // Print out final solution
    std::cout << "\n";
    grid->printSolution();

    fillSolution(grid);
}

bool initialiseGrid(SudokuGrid* grid) 
{
    int input9 = 0;
    int inputList[9] = { 0 };

    std::cout << "Please enter the sudoku in horizontal stripes";

        for (int row = 0; row < 9; row++)
        {
            system("cls");
            std::cout << "Please enter row "<< row << ":";
            std::cin >> input9;

            for (int i = 0; i < 9; i++)
            {
                inputList[8 - i] = input9 % 10;
                input9 = input9 / 10;
            }

            for (int i = 0; i < 9; i++)
            {
                grid->fillValue(row, i, inputList[i] - 1);
            }
        }    
        return true;

    // For testing
    

    /*for (int col = 0; col < 9; col++)
    {
        grid->fillValue(0, col, line0[col]-1);
        grid->fillValue(1, col, line1[col]-1);
        grid->fillValue(2, col, line2[col]-1);
        grid->fillValue(3, col, line3[col]-1);
        grid->fillValue(4, col, line4[col]-1);
        grid->fillValue(5, col, line5[col]-1);
        grid->fillValue(6, col, line6[col]-1);
        grid->fillValue(7, col, line7[col]-1);
        grid->fillValue(8, col, line8[col]-1);
    }

    return true;*/
}

void checkStraight(SudokuGrid* grid)
{
    bool temp = false;
    // Looping through all indices
    for (int row = 0; row < 9; row++) 
    {
        for (int col = 0; col < 9; col++)
        {
            // If an index is blank, check if a value can be filled there
            if (grid->returnValue(row, col) == -1)
            {
                for (int value = 0; value < 9; value++)
                {
                    temp = grid->isOneInCol(col, value) || grid->isOneInRow(row, value) || grid->isOneInBox(row, col, value);
                    // Vertical Check
                    grid->fillPossible(row, col, value, !temp);
                }

                // Checking if any horizontal pencil marks cancel anything out
                for (int k = 0; k < 2; k++)
                {
                    if (k == row % 3)
                    {
                        k++;
                    }
                    for (int value = 0; value < 9; value++)
                    {
                        grid->isExclusivelyInRow(row, col % 3, k, value);
                    }
                }

                // Checking if any vertical pencil marks cancel anything out
                for (int k = 0; k < 2; k++)
                {
                    if (k == col % 3)
                    {
                        k++;
                    }
                    for (int value = 0; value < 9; value++)
                    {
                        grid->isExclusivelyInCol(row, k, row%3, value);
                    }
                }
            }
        }
    }
}

void fillPossible(SudokuGrid* grid)
{
    int numberFilled = 0;
    bool two = 0;

    // Filling relevant boxes
    for (int row = 0; row < 9; row++) 
    {
        for (int col = 0; col < 9; col++)
        {
            for (int value = 0; value < 9; value++)
            {
                if (grid->isOnePossibleInBox(row, col, value) && grid->isOneNotNear(row, col, value) && grid->returnValue(row,col) == -1)
                {
                    grid->fillValue(row, col, value);
                    numberFilled++;
                    goto skip;
                }
                if (grid->isOnePossibleInRow(row, col, value) && grid->isOneNotNear(row, col, value) && grid->returnValue(row, col) == -1)
                {
                    grid->fillValue(row, col, value);
                    numberFilled++;
                    goto skip;
                }
                if (grid->isOnePossibleInCol(row, col, value) && grid->isOneNotNear(row, col, value) && grid->returnValue(row, col) == -1)
                {
                    grid->fillValue(row, col, value);
                    numberFilled++;
                    goto skip;
                }
                if (grid->isOnePossibleInSpot(row, col, value) && grid->returnValue(row, col) == -1)
                {
                    grid->fillValue(row, col, value);
                    numberFilled++;
                    goto skip;
                }
            }

        }
    }

    skip:

    if (numberFilled == 0)
    {
        unsolvable = 1;
    }
    if(grid->isComplete())
    {
        isComplete = 1;
    }
}

void fillSolution(SudokuGrid* grid)
{
    int numberToFill = 0;
    // Move mouse to place and click
    MouseMove(1000,200);
    sleep_for(milliseconds(1));
    LeftClick();
    sleep_for(milliseconds(10));

    // Writing in each value
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            numberToFill = grid->returnValue(row,col);
            pressNum(numberToFill);
            sleep_for(milliseconds(10));
            pressRight();
            sleep_for(milliseconds(10));
        }
        sleep_for(milliseconds(10));
        for (int i = 0; i < 9; i++)
        {
            pressLeft();
            sleep_for(milliseconds(1));
        }
        pressDown();
    }
}

////////////////////////////////////////////////////////////////
//              Moving mouse and keyboard                     //
////////////////////////////////////////////////////////////////

void LeftClick()
{
    INPUT    Input = { 0 };
    // left down 
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    ::SendInput(1, &Input, sizeof(INPUT));

    // left up
    ::ZeroMemory(&Input, sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    ::SendInput(1, &Input, sizeof(INPUT));
}

void MouseMove(int x, int y)
{
    double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
    double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
    double fx = x * (65535.0f / fScreenWidth);
    double fy = y * (65535.0f / fScreenHeight);
    INPUT  Input = { 0 };
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    Input.mi.dx = fx;
    Input.mi.dy = fy;
    ::SendInput(1, &Input, sizeof(INPUT));
}


void pressNum(int num)
{
    INPUT input[2];
    ZeroMemory(input, sizeof(input));

    input[0].type = input[1].type = INPUT_KEYBOARD;
    input[0].ki.wVk = input[1].ki.wVk = 0x30 + num + 1;

    input[1].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(2, input, sizeof(INPUT));
}

void pressRight()
{
    INPUT input[2];
    ZeroMemory(input, sizeof(input));

    input[0].type = input[1].type = INPUT_KEYBOARD;
    input[0].ki.wVk = input[1].ki.wVk = 0x27;

    input[1].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(2, input, sizeof(INPUT));
}

void pressLeft()
{
    INPUT input[2];
    ZeroMemory(input, sizeof(input));

    input[0].type = input[1].type = INPUT_KEYBOARD;
    input[0].ki.wVk = input[1].ki.wVk = 0x25;

    input[1].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(2, input, sizeof(INPUT));
}

void pressDown()
{
    INPUT input[2];
    ZeroMemory(input, sizeof(input));

    input[0].type = input[1].type = INPUT_KEYBOARD;
    input[0].ki.wVk = input[1].ki.wVk = 0x28;

    input[1].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(2, input, sizeof(INPUT));
}