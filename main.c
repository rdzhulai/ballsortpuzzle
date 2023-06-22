#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ncurses.h>
#include "ballsortpuzzle.h"

int main(){
	srand(time(NULL));
	initscr();
	cbreak();
	ball_sort_puzzle();
	getch();
	endwin();
	return 0;
}
