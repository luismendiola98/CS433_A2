/*********************************************************************************
 Name: Luis Mendiola
 Professor: Leticia Rabor
 Class: CS 433 
 Date: February 20, 2019

 Program: sudoku.c will validate a 9x9 sudoku puzzle using 27 threads.

*********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define num_threads 27

/****************************************
 Initialization and function definitions.
*****************************************/

// 9x9 Sudoku puzzle is initialized
int sudoku[9][9] =
{
	{6, 2, 4, 5, 3, 9, 1, 8, 7},
	{5, 1, 9, 7, 2, 8, 6, 3, 4},
	{8, 3, 7, 6, 1, 4, 2, 9, 5},
	{1, 4, 3, 8, 6, 5, 7, 2, 9},
	{9, 5, 8, 2, 4, 7, 3, 6, 1},
	{7, 6, 2, 3, 9, 1, 4, 5, 8},
	{3, 7, 1, 9, 5, 6, 8, 4, 2},
	{4, 9, 6, 1, 8, 2, 5, 7, 3},
};

// Array is initialized for threads to be updated to 1 if their section is valid.
int valid[num_threads] = {0};

// Struct that stores the parameters that will be passed to threads
typedef struct
{
	int row;
	int column;
}numbers;


/********************************************************************
checkColumn is a function that checks whether the column is valid.
*********************************************************************/
void *checkColumn(void* num)
{
	numbers *nums = (numbers*) num;
	int row = nums -> row;
	int col = nums -> column;

	// check if the col subsection is valid
	if(row != 0 || col > 8)
	{
		fprintf(stderr, "Invalid row or column for column subsection! row=%d, col=%d\n", row, col);
		pthread_exit(NULL);
	}

	// check if 1-9 appears once
	int validityArray[9] = {0};
	int m;

	for(m = 0; m < 9; m++)
	{
		int num = sudoku [m][col];

		if(num < 1 || num > 9 || validityArray[num - 1] == 1)
		{
			pthread_exit(NULL);
		}
		else
		{
			validityArray[num - 1] == 1;
		}
	}

	valid[18 + col] = 1;
	pthread_exit(NULL);

}

/****************************************
checkRow checks whether the row is valid.
*****************************************/
void *checkRow(void *num)
{
	numbers *nums = (numbers*) num;
	int row = nums -> row;
	int col = nums -> column;

	// check if the row subsection is valid
	if(col != 0 || row > 8)
	{
		fprintf(stderr, "Invalid row or column in row subsection! row=%d, col=%d\n", row, col);
		pthread_exit(NULL);
	}

	// ckeck if 1-9 appears once	
	int validityArray[9] = {0};
	int n;

	for(n = 0; n < 9; n++)
	{
		int num = sudoku[row][n];
		
		if(num < 1 || num > 9 || validityArray[num - 1] == 1)
		{
			pthread_exit(NULL);
		}
		else
		{
			validityArray[num - 1] = 1;
		}

	}

	valid[9 + row] = 1;
	pthread_exit(NULL);
}

/****************************************************
check3x3 checks whether the 3x3 subsection is valid.
*****************************************************/
void *check3x3(void *num)
{
	numbers *nums = (numbers*) num;
	int row = nums -> row;
	int col = nums -> column;

	// check if the 3x3 subsection is valid
	if(row > 6 || (row % 3) != 0 || col > 6 || (col % 3) != 0)
	{
		fprintf(stderr, "Invalid row or column for 3x3 subsection! row=%d, col=%d\n", row, col);
		pthread_exit(NULL);

	}

	int validityArray[9] = {0};
	int m, n;

	// ckeck if 1-9 appears once 	
	for(m = row; m < row+3; m++)
	{
		for(n = col; n < col+3; n++)
		{
			int num = sudoku[m][n];
			if(num < 1 || num > 9 || validityArray[num - 1] == 1)
			{
				pthread_exit(NULL);
			}
			else
			{
				validityArray[num - 1] = 1;
			}

		}
	
	}

	valid[ (row + col) / 3] = 1;
	pthread_exit(NULL);
}
/******************************************
 main()
*******************************************/
int main()

{

	pthread_t threads[num_threads]; // array of 27 threads
	
	int thread = 0;
	int m,n;

	// 27 threads are created. 9 for each 3x3, 9 for each row, and 9 for each column.
	for(m = 0; m < 9; m++)
	{
		for(n = 0; n < 9; n++)
		{
			if(m%3 == 0 && n%3 == 0)// checks 3x3 grid
			{
				numbers *info = (numbers *) malloc(sizeof(numbers));
				info -> row = m;
				info -> column = n;
				pthread_create(&threads[thread++], NULL, check3x3, info);
			}
			if(m == 0) // check columns
			{
				numbers *colInfo = (numbers *) malloc(sizeof(numbers));
				colInfo -> row = m;
				colInfo -> column = n;
				pthread_create(&threads[thread++], NULL, checkColumn, colInfo);

			}
			if(n == 0) // check rows
			{
				numbers *rowInfo = (numbers *) malloc(sizeof(numbers));
				rowInfo -> row = m;
				rowInfo -> column = n;
				pthread_create(&threads[thread++], NULL, checkRow, rowInfo);
			}
		}// end of for loop
	}// end of for loop

	// Wait for threads to finish.
	for(m = 0; m < num_threads; m++)
	{
		pthread_join(threads[m], NULL);

	}// end of for loop

	// Check the array to see if the sudoku puzzle is valid.
	for(m = 0; m < num_threads; m++)
	{
		if(valid[m] == 0)// if an entry is 0 then the sudoku puzzle is invalid 
		{
			printf("Sudoku solution is invalid!\n");
			return EXIT_SUCCESS;
		}
		else // otherwise it is valid
		{
			printf("Sudoku solution is valid!\n");
			return EXIT_SUCCESS;
		}

	}


}
// End of program.
