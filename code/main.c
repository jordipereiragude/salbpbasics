#include <stdio.h>
#include <stdlib.h>
#include "salbp.h"
#include "utils.h"

//../program  ../instances/SAWYER30.IN2 30 100 124587264

extern int seed;

int main(int argc, char* argv[])
{
  instance inst;
  solucion s;
  int ret,maxagrupaciones;

  if(argc<5)
  {
    printf("te falta el nombre del archivo y/o tiempo de ciclo,  max agrupaciones y numero aleatorio\n");
    return(0);
  }
  inst.c=atoi(argv[2]);
  maxagrupaciones=atoi(argv[3]);
  seed =atoi(argv[4]);
  readFile(&inst,argv[1]);
  printf("file %s ciclo %d\n",argv[1],inst.c);
  s.asignacion=generateIntVector(inst.nt+1);
  s.lista=generateIntVector(inst.nt+1);
  s.sumaD=generateIntVector(inst.nt+1);
  s.e=generateIntVector(inst.nt+1);
  s.l=generateIntVector(inst.nt+1);
  //ret=hoffmann(&inst,maxagrupaciones,&s);
  ret=greedy(&inst,&s);
  ret=busquedalocal(&inst,&s);
  /*
  free(inst.d);
  free(inst.p);
  free(inst.is);
  free(inst.ip);
  free(s.asignacion);
  free(s.lista);
  */
  return(0);
}
