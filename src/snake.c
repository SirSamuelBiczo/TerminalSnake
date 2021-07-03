#include "../includes/snake.h"


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <ncurses/curses.h>
#else 
#include <curses.h>
#endif 
#include <unistd.h>

Snake* SNAKE_Initialize(const char representation)
{
	errno = 0;
	Snake* snake = malloc(sizeof(*snake));
	
	if(snake == NULL)
	{
		fprintf(stderr, "Connot allocate %d bytes for this reason : %s", sizeof(*snake), strerror(errno));
		errno = 0;
		return NULL;
	}
	
	errno = 0;
	snake->body = malloc(sizeof(TermCoordinate));
	
	if(snake->body == NULL)
	{
		fprintf(stderr, "Connot allocate %d bytes for this reason : %s", sizeof(TermCoordinate), strerror(errno));
		errno = 0;
		free(snake);
		return NULL;
	}
	
	snake->head = &snake->body[0];
	snake->head->row = 10;
	snake->head->col = 10;
	snake->dir = RIGHT;
	snake->length = 1;
	snake->chr = representation;
	snake->apple.col = 15;
	snake->apple.row = 10;
	
	srand(time(NULL));
	
	
	return snake;
}

void SNAKE_Free(Snake* sn)
{
	free(sn->body);
	free(sn);
}

void SNAKE_Update(Snake* sn, const int key, const int term_rows, const int term_cols)
{
	int i = 0;
	bool collision = false;
	
	errno = 0;
	TermCoordinate* previous_coordinate = malloc(sizeof(*previous_coordinate) * sn->length);
	
	
	if(previous_coordinate == NULL)
	{
		fprintf(stderr, "Connot allocate %d bytes for this reason : %s", sizeof(TermCoordinate), strerror(errno));
		errno = 0;
	}
	else 
	{
		for(i = 0; i < sn->length; i++)
		{
			previous_coordinate[i] = sn->body[i];
		}
	}
	
	for(i = 1; i < sn->length; i++)
	{
		if(sn->head->row == sn->body[i].row && sn->head->col == sn->body[i].col)
		{
			collision = true;
			i = sn->length;
		}
	}
	
	if(sn->head->row < 1 || sn->head->col < 1 || sn->head->row > term_rows-2 || sn->head->col > term_cols-2 || collision)
	{
		mvprintw(term_rows/2, term_cols / 2 - strlen("GAME OVER") / 2, "GAME OVER");
		refresh();
		sleep(1);
		mvprintw(term_rows/2, term_cols / 2 - strlen("GAME OVER.") / 2, "GAME OVER.");
		refresh();
		sleep(1);
		mvprintw(term_rows/2, term_cols / 2 - strlen("GAME OVER..") / 2, "GAME OVER..");
		refresh();
		sleep(1);
		mvprintw(term_rows/2, term_cols / 2 - strlen("GAME OVER...") / 2, "GAME OVER...");
		refresh();
		sleep(1);
		free(previous_coordinate);
		SNAKE_Free(sn);
		endwin();
		exit(EXIT_SUCCESS);
	}
	
	if(sn->head->row == sn->apple.row && sn->head->col == sn->apple.col)
	{
		sn->length++;
 
		errno = 0;
		TermCoordinate* new_body = malloc(sizeof(*new_body) * sn->length);
		
		if(new_body == NULL)
		{
			fprintf(stderr, "Connot allocate %d bytes for this reason : %s", sizeof(TermCoordinate), strerror(errno));
			errno = 0;
		}
		else 
		{
			for (i = 0; i < sn->length-1; i++)
			{
				new_body[i] = sn->body[i];
			}
			sn->head = &new_body[0];
			new_body[sn->length-1].row = sn->apple.row;
			new_body[sn->length-1].col = sn->apple.col; 
			free(sn->body);
			sn->body = new_body;
			 
		}
		 
		sn->apple.row = 1 + rand() % (term_rows - 1);
		sn->apple.col = 1 + rand() % (term_cols - 1);		
	}

	if(key == KEY_LEFT && sn->dir != RIGHT)
	{
		sn->dir = LEFT;
	}
	
	if(key == KEY_RIGHT && sn->dir != LEFT)
	{
		sn->dir = RIGHT;
	}
	
	if(key == KEY_UP && sn->dir != DOWN)
	{
		sn->dir = UP;
	}
	
	if(key == KEY_DOWN && sn->dir != UP)
	{
		sn->dir = DOWN;
	}
	
	
	switch(sn->dir)
	{
		case LEFT:
			sn->head->col--;
			break;
		case RIGHT:
			sn->head->col++;
			break;
		case DOWN:
			sn->head->row++;
			break;
		case UP:
			sn->head->row--;
			break;
	}
	
	for(i = 0; i < sn->length-1; i++)
	{
		sn->body[i+1] = previous_coordinate[i];
	}
	
	free(previous_coordinate);
}

void SNAKE_Draw(Snake* sn)
{
	int i = 0;
	
	mvprintw(sn->apple.row, sn->apple.col, "O");
		
	for(i = 0; i < sn->length; i++)
	{
		TermCoordinate tmp = sn->body[i];
		mvprintw(tmp.row, tmp.col, "%c", sn->chr);
	}
}
