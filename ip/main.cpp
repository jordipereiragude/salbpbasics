#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "milp.h"
//instancia declarada aquí por comodidad
int nt;
int ne;
int* d;
int** p;

void readFile(char* nombre)
{
  FILE* in;
  int i,j,k;
  int cambio;

  printf("nombre del archivo: %s\n",nombre);
  in=fopen(nombre,"rt");
  fscanf(in,"%d\n",&nt);
  //inicio reserva memoria
  d=generateIntVector(nt+1);
  p=generateIntMatrix(nt+1,nt+1);
  //fin reserva memoria
  //limpieza
  for(i=1;i<=nt;i++)
  {
    for(j=1;j<=nt;j++)
    {
      p[i][j]=0;
    }
  }
  //leer duraciones
  for(i=1;i<=nt;i++)
  {
    fscanf(in,"%d\n",&(d[i]));
  }
  for(i=1;i<=nt;i++) printf("duración tarea %d es %d\n",i,d[i]);
  for(;;)
  {
    fscanf(in,"%d,%d\n",&i,&j);
    if(i==(-1)) break;
    printf("la tarea %d es predecesora de la tarea %d\n",i,j);
    p[i][j]=1;
  }
  fclose(in);
  return;
}

int main(int argc, char** argv)
{
  int ret,maxagrupaciones;

  if(argc<4)
  {
    printf("te falta el nombre del archivo, numero de estaciones y tiempo\n");
    return(0);
  }
  ne=atoi(argv[2]);
  readFile(argv[1]);
  solveMILP(atoi(argv[3]));
  free(d);
  free(p);
  return(0);
}
