#include "utils.h"

int seed;

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

double r8_uniform_01 (  )
/*****************************************************************************
    This routine implements the recursion
      seed = 16807 * seed mod ( 2**31 - 1 )
      unif = seed / ( 2**31 - 1 )
  Parameters:
    Input/output, int *SEED, the "seed" value.  Normally, this
    value should not be 0.  On output, SEED has been updated.
    Output, double R8_UNIFORM_01, a new pseudorandom variate, strictly between
    0 and 1.
*/
{
  int k;
  double r;

  k = seed / 127773;

  seed = 16807 * ( seed - k * 127773 ) - k * 2836;

  if ( seed < 0 )
  {
    seed = seed + 2147483647;
  }
	/*
  Although SEED can be represented exactly as a 32 bit integer,
  it generally cannot be represented exactly as a 32 bit real number!
	*/
  r = ( double ) ( seed ) * 4.656612875E-10;
  return r;
}

int r8_int (int lower,int upper)
{
  return (lower+floor(r8_uniform_01()*(double)(upper-lower+1)) );
}
