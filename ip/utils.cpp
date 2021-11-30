#include "utils.h"

//Allocates int vector of size n
int * generateIntVector(int n)
{
	int *vector;
	vector=(int* )malloc(sizeof(int)*n);
	return vector;
}

//Allocates int matrix nxm
int** generateIntMatrix(int n, int m)
{
  int i;
  int **matrix;

  if((matrix = (int**)malloc(sizeof(int) * n * m +
		      sizeof(int *) * n	 )) == NULL){
    printf("Out of memory, exit.");
    exit(1);
  }
  for ( i = 0 ; i < n ; i++ ) {
    matrix[i] = (int *)(matrix + n) + i*m;
  }
  return matrix;
}


