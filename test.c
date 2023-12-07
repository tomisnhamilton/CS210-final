/** test.c
 * =============================================================
 * Name: C3C Tomis Hamilton
 * Section:  M6
 * Project:  CS210 Final Project - TicTacCeption
 * Purpose:  Define any non-standard functions used in testing
 * ============================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "functs.h"
#include "test.h"

void runAllTests()
{
    printf("\nRunning all tests...\n");

    assert(createBoard() != NULL); // Ensure board is created
    Board *board = createBoard();

    testCheckWin(board, 1); // Test checkWin() for X
    testCheckWin(board, 2); // Test checkWin() for O

    // Initialize subWin arrays for testing checkMainWin()
    int subXWin[3][3];
    int subOWin[3][3];
    int subTie1[3][3];
    int subTie2[3][3];

    // Fill subWin arrays with X's, O's, and both (tie)
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            subXWin[i][j] = 1;
            subOWin[i][j] = 2;
            subTie1[i][j] = 3;
            if (i == 0)
            {
                if (j % 2 == 0)
                {
                    subTie2[i][j] = 1;
                }
                else
                {
                    subTie2[i][j] = 2;
                }
            }
            else
            {
                if (j % 2 == 1)
                {
                    subTie2[i][j] = 1;
                }
                else
                {
                    subTie2[i][j] = 2;
                }
            }
        }
    }

    testCheckMainWin(subXWin, 1);          // Test checkMainWin() for X
    testCheckMainWin(subOWin, 2);          // Test checkMainWin() for O
    assert(checkMainWin(subTie1, 1) == 3); // Tie
    assert(checkMainWin(subTie2, 2) == 3); // Tie

    assert(changePlayer(1) == 2); // Change player from 1 to 2
    assert(changePlayer(2) == 1); // Change player from 2 to 1

    printf("\nAll tests passed!\n");
}

void testCheckWin(Board *board, int player)
{
    // Fill a subBoard with X's, O's, and both (tie)
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board->subBoards[0][0].cells[i][j] = 'X';
            board->subBoards[1][0].cells[i][j] = 'O';
            if (i == 0)
            {
                if (j % 2 == 0)
                {
                    board->subBoards[2][0].cells[i][j] = 'X';
                }
                else
                {
                    board->subBoards[2][0].cells[i][j] = 'O';
                }
            }
            else
            {
                if (j % 2 == 1)
                {
                    board->subBoards[2][0].cells[i][j] = 'X';
                }
                else
                {
                    board->subBoards[2][0].cells[i][j] = 'O';
                }
            }
        }
    }

    if (player == 1)
    {
        assert(checkWin(board->subBoards[0], player) == 1); // X wins
    }
    else if (player == 2)
    {
        assert(checkWin(board->subBoards[1], player) == 2); // O wins
    }
    assert(checkWin(board->subBoards[2], player) == 3); // Tie
}

void testCheckMainWin(int subWin[3][3], int player)
{
    // Fill a subWin with X's, O's, and both (tie)

    if (player == 1)
    {
        assert(checkMainWin(subWin, player) == 1); // X wins
    }
    else if (player == 2)
    {
        assert(checkMainWin(subWin, player) == 2); // O wins
    }
}