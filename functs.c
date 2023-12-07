/** functs.c
 * =============================================================
 * Name: C3C Tomis Hamilton
 * Section:  M6
 * Project:  CS210 Final Project - TicTacCeption
 * Purpose:  Define any non-standard functions used in the game
 * ============================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "functs.h"

int getNumPlayers()
{
    float numPlayers = 0;
    printf("How many players are there (1-2): ");
    scanf("%f", &numPlayers);

    while (numPlayers != 1 && numPlayers != 2)
    {
        printf("\nInvalid number of players.\n");
        printf("How many players are there (1-2): ");
        scanf("%f", &numPlayers);
    }

    return numPlayers;
}

void initScreen()
{
    initscr();
    cbreak();             // don't wait for enter
    noecho();             // don't echo keypresses
    keypad(stdscr, TRUE); // enables keypad support
}

Board *createBoard()
{
    Board *board = malloc(sizeof(Board));
    int i, j, k, l;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++) // subBoard
        {
            for (k = 0; k < 3; k++)
            {
                for (l = 0; l < 3; l++) // cell
                {
                    board->subBoards[i][j].cells[k][l] = ' ';
                }
            }
        }
    }

    return board;
}

void freeBoard(Board *board)
{
    free(board);
}

void displayBoard(subBoard *board)
{
    int i, j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            mvaddch(2 * j + 1, 4 * i + 6, board->cells[i][j]);
        }
    }

    for (i = 0; i <= 6; i++)
    {
        mvaddch(i, 4, '|');
        mvaddch(i, 8, '|');
        mvaddch(i, 12, '|');
        mvaddch(i, 8, '|');
        mvaddch(i, 12, '|');
        mvaddch(i, 16, '|');

        if (i % 2 == 0)
        {
            for (j = 0; j <= 12; j++)
            {
                mvaddch(i, j + 4, '-');
            }
        }
        move(1, 6);
    }

    refresh();
}

void displayMainBoard(int subWin[3][3])
{
    int i, j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            char c = ' ';
            if (subWin[i][j] == 1)
            {
                c = 'X';
            }
            else if (subWin[i][j] == 2)
            {
                c = 'O';
            }
            else if (subWin[i][j] == 3)
            {
                c = 'T';
            }
            mvaddch(2 * j + 1, 4 * i + 6, c);
        }
    }

    for (i = 0; i <= 6; i++)
    {
        mvaddch(i, 4, '|');
        mvaddch(i, 8, '|');
        mvaddch(i, 12, '|');
        mvaddch(i, 8, '|');
        mvaddch(i, 12, '|');
        mvaddch(i, 16, '|');

        if (i % 2 == 0)
        {
            for (j = 0; j <= 12; j++)
            {
                mvaddch(i, j + 4, '-');
            }
        }
        move(1, 6);
    }

    refresh();
}

int getMove(subBoard *board, int *cellX, int *cellY, int subWin[3][3])
{
    int inputChar = 0;
    int x = 6, y = 1;

    while ((inputChar = getch()) != 'q')
    {
        switch (inputChar)
        {
        case KEY_UP:
            if (y > 1)
            {
                y -= 2;
                (*cellY)--;
            }
            break;
        case KEY_DOWN:
            if (y < 5)
            {
                y += 2;
                (*cellY)++;
            }
            break;
        case KEY_LEFT:
            if (x > 6)
            {
                x -= 4;
                (*cellX)--;
            }
            break;
        case KEY_RIGHT:
            if (x < 14)
            {
                x += 4;
                (*cellX)++;
            }
            break;
        case ' ': // spacebar
            if (board->cells[*cellX][*cellY] == ' ')
            {
                if (subWin[*cellX][*cellY] != 0)
                {
                    mvprintw(11, 0, "That board has already been won, staying on current.");
                    return 1;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                mvprintw(11, 0, "Invalid move, try again.");
                mvprintw(12, 0, "%c", board->cells[*cellX][*cellY]);
                return 0;
            }
            break;
        default:
            break;
        }

        move(y, x);
        refresh();
    }
    return 0;
}

void updateBoard(Board *board, FILE *inFile, int *boardX, int *boardY, int *cellX, int *cellY, int player, int currentBoard, int subWin[3][3])
{
    if (player == 1)
    {
        board->subBoards[*boardX][*boardY].cells[*cellX][*cellY] = 'X';
    }
    else
    {
        board->subBoards[*boardX][*boardY].cells[*cellX][*cellY] = 'O';
    }

    writeRecord(inFile, board, cellX, cellY, subWin, player, currentBoard);

    if ((subWin[*boardX][*boardY] = checkWin(&board->subBoards[*boardX][*boardY], player)) != 0)
    {
        if (subWin[*boardX][*boardY] == 1)
        {
            mvprintw(12, 0, "Player 1 wins this board!");
            fprintf(inFile, "Player 1 has won board %d.\n\n", currentBoard + 1);
        }
        else if (subWin[*boardX][*boardY] == 2)
        {
            mvprintw(12, 0, "Player 2 wins this board!");
            fprintf(inFile, "Player 2 has won board %d.\n\n", currentBoard + 1);
        }
        else if (subWin[*boardX][*boardY] == 3)
        {
            mvprintw(12, 0, "It's a tie!");
            fprintf(inFile, "Board %d was a tie.\n\n", currentBoard + 1);
        }
    }

    if (subWin[*cellX][*cellY] == 0)
    {
        *boardX = *cellX;
        *boardY = *cellY;
    }
    if (subWin[*boardX][*boardY] != 0)
    {
        while (subWin[*boardX][*boardY] != 0)
        {
            *boardX = rand() % 3;
            *boardY = rand() % 3;
        }
    }
    *cellX = 0;
    *cellY = 0;
}

int changePlayer(int player)
{
    if (player == 1)
    {
        player = 2;
    }
    else
    {
        player = 1;
    }

    return player;
}

int checkWin(subBoard *board, int player)
{
    int i, j;
    int win = 0; // if 1 then player 1 wins, if 2 then player 2 wins, if 3 then tie

    // check rows
    for (i = 0; i < 3; i++)
    {
        if (board->cells[i][0] == board->cells[i][1] && board->cells[i][1] == board->cells[i][2] && board->cells[i][0] != ' ')
        {
            if (board->cells[i][0] == 'X')
            {
                win = 1;
            }
            else if (board->cells[i][0] == 'O')
            {
                win = 2;
            }
        }
    }

    // check columns
    for (j = 0; j < 3; j++)
    {
        if (board->cells[0][j] == board->cells[1][j] && board->cells[1][j] == board->cells[2][j] && board->cells[0][j] != ' ')
        {
            if (board->cells[0][j] == 'X')
            {
                win = 1;
                mvprintw(11, 0, "Player 1 wins!");
            }
            else if (board->cells[0][j] == 'O')
            {
                win = 2;
                mvprintw(11, 0, "Player 2 wins!");
            }
        }
    }

    // check diagonals
    if (board->cells[0][0] == board->cells[1][1] && board->cells[1][1] == board->cells[2][2] && board->cells[0][0] != ' ')
    {
        if (board->cells[0][0] == 'X')
        {
            win = 1;
            mvprintw(11, 0, "Player 1 wins!");
        }
        else if (board->cells[0][0] == 'O')
        {
            win = 2;
            mvprintw(11, 0, "Player 2 wins!");
        }
    }
    else if (board->cells[0][2] == board->cells[1][1] && board->cells[1][1] == board->cells[2][0] && board->cells[0][2] != ' ')
    {
        if (board->cells[0][2] == 'X')
        {
            win = 1;
            mvprintw(11, 0, "Player 1 wins!");
        }
        else if (board->cells[0][2] == 'O')
        {
            win = 2;
            mvprintw(11, 0, "Player 2 wins!");
        }
    }

    // check for tie
    if (win == 0)
    {
        int tie = 1;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {

                if (board->cells[i][j] == ' ')
                {
                    tie = 0;
                }
            }
        }
        if (tie == 1)
        {
            win = 3;
            mvprintw(11, 0, "It's a tie!");
        }
    }

    return win;
}

int checkMainWin(int subWin[3][3], int player)
{
    int i, j;
    int win = 0; // if 1 then player 1 wins, if 2 then player 2 wins, if 3 then tie

    // check rows
    for (i = 0; i < 3; i++)
    {
        if (subWin[i][0] == subWin[i][1] && subWin[i][1] == subWin[i][2] && subWin[i][0] != 0)
        {
            if (subWin[i][0] == 1)
            {
                win = 1;
            }
            else if (subWin[i][0] == 2)
            {
                win = 2;
            }
        }
    }

    // check columns
    for (j = 0; j < 3; j++)
    {
        if (subWin[0][j] == subWin[1][j] && subWin[1][j] == subWin[2][j] && subWin[0][j] != 0)
        {
            if (subWin[0][j] == 1)
            {
                win = 1;
            }
            else if (subWin[0][j] == 2)
            {
                win = 2;
            }
        }
    }

    // check diagonals
    if (subWin[0][0] == subWin[1][1] && subWin[1][1] == subWin[2][2] && subWin[0][0] != 0)
    {
        if (subWin[0][0] == 1)
        {
            win = 1;
        }
        else if (subWin[0][0] == 2)
        {
            win = 2;
        }
    }
    else if (subWin[0][2] == subWin[1][1] && subWin[1][1] == subWin[2][0] && subWin[0][2] != 0)
    {
        if (subWin[0][2] == 1)
        {
            win = 1;
        }
        else if (subWin[0][2] == 2)
        {
            win = 2;
        }
    }

    // check for tie
    int tieCount = 0;
    if (win == 0)
    {
        for (int i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {

                if (subWin[i][j] != 0)
                {
                    tieCount++;
                }
                else
                {
                    tieCount = 0;
                }
            }
        }
        if (tieCount == 9)
        {
            win = 3;
        }
    }

    return win;
}

void writeRecord(FILE *inFile, Board *board, int *cellX, int *cellY, int subWin[3][3], int player, int currentBoard)
{
    fprintf(inFile, "The current board is %d\n", currentBoard + 1);
    fprintf(inFile, "Player %d's move: %d %d\n", player, *cellX, *cellY);
    fprintf(inFile, "\n");
}

void playGame(int numPlayers)
{
    srand(time(0)); // seed random number generator
    char fileName[20] = "game_results.txt";
    FILE *inFile = fopen(fileName, "w+");
    fprintf(inFile, "Tic-Tac-Ception Game Results:\n\n");
    fprintf(inFile, "Number of players: %d\n\n", numPlayers);

    int mainWin = 0;
    int subWin[3][3];
    for (int i = 0; i < 3; i++) // initialize subWin to 0
    {
        for (int j = 0; j < 3; j++)
        {
            subWin[i][j] = 0;
        }
    }
    int player = 1;
    int currentBoard = 0;
    int boardX = 0, boardY = 0;
    int cellX = 0, cellY = 0;

    initScreen();

    Board *board = createBoard();
    displayBoard(&board->subBoards[boardX][boardY]);

    // Random Initial Board Selection
    boardX = rand() % 3;
    boardY = rand() % 3;
    currentBoard = boardX + boardY * 3;
    mvprintw(11, 0, "The Board is %d", currentBoard + 1);
    mvprintw(14, 0, "Press 'q' to quit.");
    move(1, 6);

    // Gameplay Loop
    while (mainWin == 0 && getch() != 'q')
    {
        move(1, 6);
        if (numPlayers == 2)
        {
            if (getMove(&board->subBoards[boardX][boardY], &cellX, &cellY, subWin) == 1)
            {
                clear();

                updateBoard(board, inFile, &boardX, &boardY, &cellX, &cellY, player, currentBoard, subWin);
                if (checkMainWin(subWin, player) != 0)
                {
                    mainWin = checkMainWin(subWin, player);
                    break;
                }

                displayBoard(&board->subBoards[boardX][boardY]);

                player = changePlayer(player);
                mvprintw(10, 0, "Player %d's turn", player);

                currentBoard = boardX + boardY * 3;
                mvprintw(11, 0, "The Board is %d", currentBoard + 1);
                mvprintw(14, 0, "Press 'q' (twice) to quit.");

                refresh();
            }
            else
            {
                cellX = 0;
                cellY = 0;
            }
        }
        else if (numPlayers == 1)
        {
            if (player == 1)
            {
                if (getMove(&board->subBoards[boardX][boardY], &cellX, &cellY, subWin) == 1)
                {
                    clear();

                    updateBoard(board, inFile, &boardX, &boardY, &cellX, &cellY, player, currentBoard, subWin);
                    if (checkMainWin(subWin, player) != 0)
                    {
                        mainWin = checkMainWin(subWin, player);
                        break;
                    }

                    displayBoard(&board->subBoards[boardX][boardY]);

                    player = changePlayer(player);
                    mvprintw(10, 0, "Player %d's turn", player);

                    currentBoard = boardX + boardY * 3;
                    mvprintw(11, 0, "The Board is %d", currentBoard + 1);
                    mvprintw(14, 0, "Press 'q' (twice) to quit.");

                    refresh();
                }
                else
                {
                    cellX = 0;
                    cellY = 0;
                }
            }
            else if (player == 2)
            {
                // AI
                while (board->subBoards[boardX][boardY].cells[cellX][cellY] != ' ') // random move
                {
                    cellX = rand() % 3;
                    cellY = rand() % 3;
                }

                updateBoard(board, inFile, &boardX, &boardY, &cellX, &cellY, player, currentBoard, subWin);
                if (checkMainWin(subWin, player) != 0)
                {
                    mainWin = checkMainWin(subWin, player);
                    break;
                }
                displayBoard(&board->subBoards[boardX][boardY]);
                currentBoard = boardX + boardY * 3;
                mvprintw(11, 0, "The Board is %d", currentBoard + 1);
                player = changePlayer(player);
                mvprintw(10, 0, "Player %d's turn", player);
            }
        }
    }
    clear();
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(1));

    displayMainBoard(subWin);
    mvprintw(9, 3, "MAIN BOARD VIEW");
    if (mainWin == 1)
    {
        mvprintw(10, 3, "Player 1 wins!");
        fprintf(inFile, "Player 1 has won this game.\n");
    }
    else if (mainWin == 2)
    {
        mvprintw(10, 3, "Player 2 wins!");
        fprintf(inFile, "Player 2 has won this game.\n");
    }
    else if (mainWin == 3)
    {
        mvprintw(10, 3, "It's a tie!");
        fprintf(inFile, "This game was a tie.\n");
    }
    else
    {
        fprintf(inFile, "This game was not finished.\n");
    }
    mvprintw(12, 3, "Thanks for playing!");
    mvprintw(13, 3, "Press any key to exit...");
    getch();
    fclose(inFile);
    freeBoard(board);
    endwin();
}