#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED

#include "termcoordinate.h"
#include "direction.h"

typedef struct 
{
	Direction dir;
	TermCoordinate* head;
	TermCoordinate* body;
	TermCoordinate apple;
	unsigned int length;
	char chr;
}Snake;

Snake* SNAKE_Initialize(const char representation);
void SNAKE_Free(Snake* sn);
 
void SNAKE_Update(Snake* sn, const int key, const int term_rows, const int term_cols);
void SNAKE_Draw(Snake* sn);


#endif 
