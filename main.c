/** main.c
 * =============================================================
 * Name: C3C Tomis Hamilton
 * Section:  M6
 * Project:  CS210 Final Project - TicTacCeption
 * Purpose:  Main file for TicTacCeption game, meant to replicate Ultimate Tic-Tac-Toe
 * Documentation Statement:  Please read documentation.txt
 *
 * REQUIREMENTS NOT MET (REMOVED):
 * - Asking for player names
 * - Player initially selects the starting subBoard
 * - Allocate memory for the subBoards
 *
 * REQUIREMENTS MODIFIED:
 * - Draws each board (3x3), no 9x9 drawing
 * - Changed how updateBoard is handled
 * - checkWin has an additional parameter and checks for subBoards only
 * - playerMove is now getMove and has additional parameters
 *
 * REQUIREMENTS ADDED:
 * - Added AI functionality for single player
 *
 * COMPILATION AND RUNNING INSTRUCTIONS:
 * - Compilation: gcc main.c functs.c test.c -Wall -Werror -o tictac -lcurses
 * - Running: ./tictac
 * ============================================================= */

#include <stdio.h>
#include <stdlib.h>
#include "functs.h"
#include "test.h"

int main()
{
    int choice = 0;
    printf("Welcome to Tic-Tac-Ception!\n\n");
    printf("1. Play Game\n");
    printf("2. Run Tests\n");
    printf("3. Exit\n");
    printf("\nEnter your choice: ");

    while (choice != 1 && choice != 2 && choice != 3)
    {
        scanf("%d", &choice);
        if (choice != 1 && choice != 2 && choice != 3)
        {
            printf("Invalid choice. Please enter a valid choice: ");
        }
    }

    if (choice == 1)
    {
        int numPlayers = getNumPlayers();
        playGame(numPlayers);
        return 0;
    }
    else if (choice == 2)
    {
        runAllTests();
        return 0;
    }
    else if (choice == 3)
    {
        return 0;
    }

    return 0;
}