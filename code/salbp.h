#include <stdio.h>
#include <stdlib.h>

typedef struct instance
{
  int nt; //numero de tareas
  int c; //tiempo de ciclo
  int* d; //duración de las tareas
  //precedencias
  int** p; // p[i][j]=1 si la tarea i es predecesora de la j
  int** ip; // ip[i][0] indica el númeo de predecesoras inmediatas de la tarea i 
            // ip[i][j] indica la j-ésima predecesora inmediata de la tarea i
  int** is; // idem  pero con sucesoras
}instance;

typedef struct dataHoffmann
{
  //la estación que estoy haciendo
  int *enConstruccion;
  //vector de candidatas y de pendientes
  int* candidatas;
  int* precPendientes;
  int numAgrupaciones;

  int MAX_AGRUPACIONES;
}dataHoffmann;

typedef struct solucion
{
  int* asignacion; // asignacion[i], en qué estación se hace la tarea i
  int* lista;
  int numEstaciones;
  int* sumaD;
  int c; //valor maximo de sumaD -> objetivo que voy a intentar minimizar en busquedalocal
  int* e; //estacion earliest a la que puedo cambiar una tarea
  int* l; //estacion latest a la que puedo cambiar una tarea
}solucion;

int readFile(instance* inst,char* nombre);
int hoffmann(instance* inst,int numMaxAgrupaciones,solucion* s);
int greedy(instance* inst,solucion* s);
int busquedalocal(instance* inst,solucion* s);
