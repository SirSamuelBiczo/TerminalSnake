#include "../includes/snake.h"

#ifdef _WIN32
#include <ncurses/curses.h>
#else 
#include <curses.h>
#endif 
#include <stdbool.h>
 
int main(void)
{
	Snake* snake = SNAKE_Initialize('*');
	int keycode = 0;
	int cols = 0, rows = 0; 
	
	initscr();
	noecho();
	curs_set(false);
	keypad(stdscr, true);
	timeout(500);
	
	getmaxyx(stdscr, rows, cols);
 
	while((keycode = getch()) != 27)
	{
		refresh();
		erase();
		box(stdscr, ACS_VLINE, ACS_HLINE);
		SNAKE_Update(snake, keycode, rows, cols);
		SNAKE_Draw(snake);
	}
	
	
	
	SNAKE_Free(snake);
	endwin();
 
 	
	return 0;
}
