/** functs.h
 * =============================================================
 * Name: C3C Tomis Hamilton
 * Section:  M6
 * Project:  CS210 Final Project - TicTacCeption
 * Purpose:  Declare any non-standard functions used in the game
 * ============================================================= */

#ifndef FUNCTS_H
#define FUNCTS_H

#include <stdio.h>
#include <stdlib.h>

// Structs
typedef struct subBoard
{
    char cells[3][3];
} subBoard;

typedef struct Board
{
    subBoard subBoards[3][3];
} Board;

// Function Declarations
int getNumPlayers();

void initScreen();
void displayBoard(subBoard *board);
void displayMainBoard(int subWin[3][3]);

Board *createBoard();
void freeBoard(Board *board);
void updateBoard(Board *board, FILE *inFile, int *boardX, int *boardY, int *cellX, int *cellY, int player, int currentBoard, int subWin[3][3]);

int getMove(subBoard *board, int *cellX, int *cellY, int subWin[3][3]);

int checkWin(subBoard *board, int player);
int checkMainWin(int subWin[3][3], int player);

int changePlayer(int player);

void writeRecord(FILE *inFile, Board *board, int *cellX, int *cellY, int subWin[3][3], int player, int currentBoard);

void playGame(int numPlayers);

#endif