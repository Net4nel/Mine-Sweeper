// Name: Netanel Amiel
// ID: 303136972

#include <stdio.h>
#include <stdlib.h>
#include "mineSweeper.h"
#include "colorPrint.h"
#include <time.h>

bool initBoard(GameBoard *g, int rows, int cols, int level)
{
	int i,j;
	if(rows<=0 || rows>MAX_BOARD_DIM)
		return FALSE;
	if(cols<=0 || cols>MAX_BOARD_DIM)
		return FALSE;
	if(level<EASY || level>HARD)
		return FALSE;
	
	g->hiddenTiles = (rows * cols) ;
	
	g->rows=rows;
	g->cols=cols;
	
	g->board = (Tile**)malloc(sizeof(Tile*)*rows);
	
	if(g->board == NULL)
		return;
	for(i=0;i<rows;i++)
	{
		g->board[i] = (Tile*)malloc(sizeof(Tile)*cols);
			if(g->board[i] == NULL)
				return;
			for(j=0;j<cols;j++)
			{
			g->board[i][j].isVisible = FALSE;
			g->board[i][j].isMine = FALSE;
			g->board[i][j].isFlagged = FALSE;
			}
		
	}
	populateMines(g,level);
	markNumbers(g);
}

void populateMines(GameBoard *g, int level)
{
	int i,ro=0,co=0;
	srand(time(0));
	
	if(level==EASY)
		g->totalMines = ((g->rows)*(g->cols)*EASY_LEVEL_PERCENT)/100;
		
	else if(level==MEDIUM)
		g->totalMines = ((g->rows)*(g->cols)*MEDIUM_LEVEL_PERCENT)/100;
	
	else
		g->totalMines = ((g->rows)*(g->cols)*HARD_LEVEL_PERCENT)/100;
			
	
	for(i=g->totalMines;i>0;i--)
	{
		ro = rand()%(g->rows);
		co = rand()%(g->cols);
		
		
		if(g->board[ro][co].isMine == TRUE)
		{
			i++;
		}
		else
		{
			g->board[ro][co].isMine = TRUE;
			g->board[ro][co].numOfMines = -1;
		}
	}
}

void markNumbers(GameBoard *g)
{
	int i,j;
	for(i=0;i<(g->rows);i++)
	{
		for(j=0;j<(g->cols);j++)
		{
			if(g->board[i][j].isMine)
				g->board[i][j].numOfMines = -1;
			else
			{
				if((i-1)>=0 && (j-1)>=0)
					if(g->board[i-1][j-1].isMine)
						g->board[i][j].numOfMines++;
			
				if(i>=0 && j-1>=0)
					if(g->board[i][j-1].isMine)
						g->board[i][j].numOfMines++;
				
				if(i+1<=g->rows-1 && j-1>=0)
					if(g->board[i+1][j-1].isMine)
						g->board[i][j].numOfMines++;
				
				if(i+1<=g->rows-1 && j<=g->cols-1)
					if(g->board[i+1][j].isMine)
						g->board[i][j].numOfMines++;
				
				if(i+1<=g->rows-1 && j+1<=g->cols-1)
					if(g->board[i+1][j+1].isMine)
						g->board[i][j].numOfMines++;
			
				if(i<=g->rows-1 && j+1<=g->cols-1)
					if(g->board[i][j+1].isMine)
						g->board[i][j].numOfMines++;
				
				if(i-1>=0 && j+1<=g->cols-1)
					if(g->board[i-1][j+1].isMine)
						g->board[i][j].numOfMines++;
				
				if(i-1>=0 && j<=g->cols-1)
					if(g->board[i-1][j].isMine)
						g->board[i][j].numOfMines++;
			}
		}
	}
}

void clickTile(GameBoard *g,int row,int col)
{
	int i=row;
	int j=col;
		
	if(g->board[row][col].isVisible)
		return;
	if(g->board[row][col].isFlagged)
		return;
	
	//if i clicked in mine i want the feature mineClicked set to true.
	if(g->board[row][col].isMine)
	{
	 	g->board[row][col].isVisible=TRUE;
		g->isMineClicked=TRUE;
	}
	else // Tile is not a mine.
	{
		g->board[row][col].isVisible=TRUE;
		g->hiddenTiles--;
		//start the recursion
		if(g->board[row][col].numOfMines==0)
		{
			if(i-1>=0&&j-1>=0)
				clickTile(g,i-1,j-1);	
			if(i-1>=0)
				clickTile(g,i-1,j);	
			if(i-1>=0&&j+1<g->cols)
				clickTile(g,i-1,j+1);
			if(j-1>=0)
				clickTile(g,i,j-1);
			if(j+1<g->cols)
				clickTile(g,i,j+1);
			if(i+1<g->rows&&j-1>=0)
				clickTile(g,i+1,j-1);
			if(i+1<g->rows&&j+1<g->cols)
				clickTile(g,i+1,j+1);
			if(i+1<g->rows)
				clickTile(g,i+1,j);
		}
	}
}

void flagTile(GameBoard *g, int row, int col)
{
	if(g->board[row][col].isVisible)
		return;
	else
	{
		if(g->board[row][col].isFlagged)
			g->board[row][col].isFlagged = FALSE;
		else
			g->board[row][col].isFlagged = TRUE;
	}
}

void printBoard(GameBoard *g, int cursorCoords[2])
{
int i,j;
	for(i=0;i<g->rows;i++) //MAIN PRINT
	{
		for(j=0;j<g->cols;j++)
		{
			if(i==cursorCoords[0] && j==cursorCoords[1] && !isWinOrLost(g))
			{
				if(!g->board[i][j].isVisible)
				{
					if (g->board[i][j].isFlagged)
						colorPrint(FG_White, BG_Blue, ATT_Bright, "|>");
					else
						colorPrint(FG_White, BG_Blue, ATT_Bright, "  ");
				}
				else
				{
					if (g->board[i][j].numOfMines>0)
					{
						char c[10];
						sprintf(c,"%d",g->board[i][j].numOfMines);
						colorPrint(FG_White, BG_Green, ATT_Bright, " ");
						colorPrint(FG_White, BG_Green, ATT_Bright, c);
					}
					else if(g->board[i][j].numOfMines==0)
					{
						colorPrint(FG_White, BG_Green, ATT_Bright, "  ");
					}
				}
			}
			else
			{
				if(!g->board[i][j].isVisible)
				{
					if(g->board[i][j].isFlagged)
						colorPrint(FG_Yellow, BG_Yellow, ATT_Bright, "|>");
					else
						colorPrint(FG_White, BG_White, ATT_Bright, "  ");
				}
				else
				{
					if(g->board[i][j].isMine)
						colorPrint(FG_White, BG_Red, ATT_Bright, " *");
					else if (g->board[i][j].numOfMines>0)
					{
						char c[10];
						sprintf(c,"%d",g->board[i][j].numOfMines);
						colorPrint(FG_White, BG_Cyan, ATT_Bright, " ");
						colorPrint(FG_Cyan, BG_Cyan, ATT_Bright, c);
					}
					else
						colorPrint(FG_White, BG_Cyan, ATT_Bright, "  ");
				}
			}
		}
	printf("\n");
	
	}
}

// PRIVATE FNCTIONS:

bool isWinOrLost(GameBoard *g)
{
	if (g->isMineClicked)
		return TRUE;
	if (g->hiddenTiles == g->totalMines)
		return TRUE;
	else
		return FALSE;
}

void freeMemory(GameBoard *g)
{
	int i;
	int end = g->rows-1;
	for (i = end; i>=0; i--)
		free(g->board[i]);
	free(g->board);
	free(g);
}




