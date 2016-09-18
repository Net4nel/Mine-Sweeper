// Name: Netanel Amiel
// ID: 303136972

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mineSweeper.h"
#include "colorPrint.h"
#include "getch.h"

int main()
{	
	int ro,co,level;
	char c=NULL;

	int a[2];
	a[0]=0;
	a[1]=0;
	GameBoard *g = (GameBoard*)malloc(sizeof(GameBoard));
	
	printf("Please enter num of rows:");
	scanf("%d",&ro);
	
	printf("Please enter num of columns:");
	scanf("%d",&co);

	printf("Please enter level [1-3]:");
	scanf("%d",&level);
	
	if(!initBoard(g,ro,co,level))
	{
		printf("Sorry, there was a problem creating the board. Try again");
		return;
	}
	getch();
	while(c!=QUIT)
	{
		clearScreen();
		printBoard(g,a);
		
		printf("Press 'q' anytime to quit the program\n\n");
		c = getch();
		
		if(c == CLICK_TILE)
		{
			clickTile(g,a[0],a[1]);
			clearScreen();
			printBoard(g,a);
			if(g->isMineClicked)
			{
				printf("GAME OVER! :(\n\n");
				break;
			}
			else if(g->totalMines == g->hiddenTiles)
			{
				printf("YOU WIN!\n\n");
				break;
			}
		}
		else if(c == QUIT)
		{
			break;
		}
		else if(c == UP)
		{
			if(a[0]>0)
				a[0]--;
		}
		else if(c == DOWN)
		{
			if(a[0]<ro-1)
				a[0]++;
		}
		else if(c == LEFT)
		{
			if(a[1]>0)
				a[1]--;
		}
		else if(c == RIGHT)
		{
			if(a[1]<co-1)
				a[1]++;
		}
		else if(c == FLAG_TILE)
		{
			flagTile(g,a[0],a[1]);
		}
	}
	freeMemory(g);
}
