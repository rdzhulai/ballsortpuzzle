#include <stdio.h>
#include <stdlib.h> 
#include "ballsortpuzzle.h"
#include <ncurses.h>
#include <stdbool.h>

void generator(const int rows, const int columns, char field[rows][columns])
{
    int blank_columns_indexes[BLANK_COLUMNS];
     
        for(int i = 0; i < BLANK_COLUMNS; i++)
        {
                blank_columns_indexes[i] = rand() % columns;
                for(int j = 0; j < i; j++)
                {
                        if(blank_columns_indexes[i] == blank_columns_indexes[j])
                        {
                                i--;
                                break;
                        }
                }
        }

        char symbols[columns - BLANK_COLUMNS];
        for(int i = 0; i < columns - BLANK_COLUMNS; i++)
        {
            symbols[i] = (rand() % 26) + 'a';
            for(int j = 0; j < i; j++)
            {
                if(symbols[j] == symbols[i]){
                    i--;
                    break;
                }
            }
        }

        for(int i = 0, j = 0; i < columns; i++) 
        {
            bool is_filled = false;
            for(int j = 0; j < BLANK_COLUMNS; j++)
            {
                    if(i == blank_columns_indexes[j])
                    {
                            is_filled = true;
                            for(int k = 0; k < rows; k++)
                                field[k][i] = BLANK;
                    }
            }
            if (!is_filled)
            {
                for(int k = 0; k < rows; k++)
                {
                    field[k][i] = symbols[j];
                    if(++j == columns - BLANK_COLUMNS) j = 0;               
                }

            }
        }

}

bool check(const int rows, const int columns, char field[rows][columns])
{
    for (int i = 0; i < columns; i++)
    {   
        int j = rows - 1;
        char c = field[j--][i];
        for (; j >= 0; j--)
        {
            if(field[j][i] != c) return false;
        }
        
    }
    return true;
}

void down_possible(const int rows, const int columns, char field[rows][columns], int x, int y)
{
    if(x < 1 || x > columns || y < 1 || y > columns || x == y)
    {
        printw("Invalid numbers.\n");
        return;
    }
    
    int i = 0;
    while(field[i][y-1] == BLANK && i < rows) 
        i++;
    if(i == 0 && field[i][y-1] != BLANK)
    {
        printw("The column is full.\n");
        return;
    }

    int j = 0;
    while(field[j][x-1] == BLANK && j < rows) 
        j++;

    if(j == rows)
    {
        printw("The column is empty.\n");
        return;
    }
    if(i != rows && field[i][y-1] != field[j][x-1])
    {
        printw("Must not be the same.\n");
        return;
    }
    field[--i][y-1] = field[j][x-1];
    field[j][x-1] = BLANK;
}




void game_field(const int rows, const int columns, char field[rows][columns])
{
    const int x = stdscr->_maxx / 2 - (columns / 2 * 7 - 3); 
    int y = Y;

    move(y, x);
    for(int i = 0; i < rows; i++)
    {   
        printw("%d.", i + 1);
        move(y, x + 3);
        for(int j = 0; j < columns; j++)
        {
            printw("|");
            attron(A_BOLD);
            printw("  %c  ", field[i][j]);
            attroff(A_BOLD);
        }
        printw("|");   
        move(++y, x); 
    }

    for(int i = 0; i < columns; i++)
    {
        move(y, x + 4 + i * 6);
        printw("*****");
    }

    move(++y, x);
    for(int i = 0; i < columns; i++)
    {
        move(y, x + 6 + i * 6);
        printw("%d.", i + 1);
    }
}

void ball_sort_puzzle()
{
    const int x = stdscr->_maxx / 2 - (COLUMNS / 2 * 7 - 3);
    const int y = Y + ROWS + 3;
    char field[ROWS][COLUMNS];

    generator(ROWS, COLUMNS, field);
    game_field(ROWS, COLUMNS, field);
    refresh();

    do{
        int mv_from, mv_to;
        move(y, x);
        printw("Move from column: ");
        refresh();
        scanw("%d", &mv_from);
        move(y + 1, x);
        printw("To column: ");
        refresh();
        scanw("%d", &mv_to);
        move(y + 2, x);
        down_possible(ROWS, COLUMNS, field, mv_from, mv_to);
        refresh();
        getch();
        clear();
        game_field(ROWS, COLUMNS, field);
        refresh();
    }while(!check(ROWS, COLUMNS, field));

    clear();
    move(y, x);
    printw("Congrats! You won the game!");
    refresh();
}