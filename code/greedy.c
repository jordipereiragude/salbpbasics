#include <stdio.h>
#include <stdlib.h>
#include "salbp.h"
#include "utils.h"

int greedy(instance* inst,solucion* s)
{
  int i,j;
  int numTareasAsignadas=0;
  int tiempoLibre=inst->c;
  int bestCandidate;
  int bestCandidateValue;
  int precedenciasPendientes[inst->nt+1];
  int prioridad[inst->nt+1];

  printf("entro en greedy\n");
  //inicializar a 0 la solución
  s->numEstaciones=1;
  for(i=1;i<=inst->nt;i++) s->asignacion[i]=0;
  for(i=1;i<=inst->nt;i++) precedenciasPendientes[i]=inst->ip[i][0];
  for(i=1;i<=inst->nt;i++) prioridad[i]=r8_int(1,10000000);
  for(;numTareasAsignadas<inst->nt; )
  {
    //inicialización antes del bucle
    bestCandidate=0;
    bestCandidateValue=0;
    //buscamos una tarea que quepa en el tiempo libre actual
    for(i=1;i<=inst->nt;i++)
    {
      //¿es candidata? 2 condiciones, precedencias y tiempo
      if((precedenciasPendientes[i]==0)&&(tiempoLibre>=inst->d[i]))
      {
        //entre las candidatas, me voy a quedar con la tarea que más dure
        //if(bestCandidateValue<inst->d[i]) //regla greedy ESTO ES LO QUE HAY QUE CAMBIAR PARA HACERLO AL AZAR
        if(bestCandidateValue<prioridad[i]) //regla greedy ESTO ES LO QUE HAY QUE CAMBIAR PARA HACERLO AL AZAR
        {
          bestCandidate=i;
          bestCandidateValue=prioridad[i];
        }
      }
    }
    //2 cosas, caso 1: bestCandidate=0 ; bestCandidate!=0
    if(bestCandidate!=0)
    {
      printf("asigno la tarea %d a la estacion %d\n",bestCandidate,s->numEstaciones);
      //si la encontramos la asignamos a la estación en curso
      s->asignacion[bestCandidate]=s->numEstaciones;
      precedenciasPendientes[bestCandidate]=(-1);
      tiempoLibre = tiempoLibre - inst->d[bestCandidate];
      for(j=1;j<=inst->is[bestCandidate][0];j++)
      {
        precedenciasPendientes[inst->is[bestCandidate][j]]--;
      }
      numTareasAsignadas++;
      s->lista[numTareasAsignadas]=bestCandidate;
    }
    else
    {
      //si no la encontramos cerramos la estación y creamos una nueva
      printf("ya no caben más tareas en la estacion %d\n",s->numEstaciones);
      s->numEstaciones++;
      tiempoLibre=inst->c;
    }
  }
  return(0);
}

