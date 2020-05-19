#include "salbp.h"
#include "utils.h"
#include <assert.h>

//solucionCompleta
int *asignacionSolucion; //la solucion en construccion

//la mejor asignacion para la estación en curso
int *bestStation; //las tareas de la mejor hasta el momento
int numBestStation; //numero de tareas en bestStation
int bestStationValue; //valor de la mejor hasta el momento (menor tiempo libre)

void imprimirSolutionHastaAhora(int numTareas,dataHoffmann *data)
{
  int i;
  printf("(%d) %d\t",data->numAgrupaciones,numTareas);
  for(i=1;i<numTareas;i++) printf("%d ",data->enConstruccion[i]);
  printf("\n");
}

int recursion(instance* inst,int depth,int tiempoLibre,int ini,int numCandidatas,dataHoffmann *data)
{
  int i,ii;
  int j;
  int estacionCompleta=1;

  if(bestStationValue==0) return(0);
  if(data->numAgrupaciones==data->MAX_AGRUPACIONES) return(0);
  /*
  printf("  nivel %d\n",depth);
  */
  //para todas las tareas candidatas
  for(ii=ini;ii<=numCandidatas;ii++)
  {
    i=data->candidatas[ii];
    /*
    printf("   mirando tarea %d en posicion %d\n",i,ii);
    */
    //ver si cabe (por tiempo)
    if(inst->d[i]<=tiempoLibre)
    {
      /*
      printf("    la tarea %d cabe\n",i);
      //si cabe (la incluyo en la estacion)
      printf("he asignado la tarea %d\t",i);
      */
      tiempoLibre=tiempoLibre-inst->d[i];
      data->enConstruccion[depth]=i;
      /*
      printf("tiempoLibre %d\n",tiempoLibre);
      */
      //actualizar precedencias
      for(j=1;j<=inst->is[i][0];j++)
      {
        data->precPendientes[inst->is[i][j]]--;
        if(data->precPendientes[inst->is[i][j]]==0)
        {
          numCandidatas++;
          data->candidatas[numCandidatas]=inst->is[i][j];
        }
      }
      /*
      printf("son candidatas: ");
      for(j=1;j<=numCandidatas;j++) printf("%d\t",data->candidatas[j]);
      printf("\n");
      */
      //llamo a recursion(depth+1,tiempoLibre, , );
      estacionCompleta=0;
      recursion(inst,depth+1,tiempoLibre,ii+1,numCandidatas,data);
      //si cabe (la borro de la estacion)
      data->precPendientes[inst->is[i][j]]++;
      for(j=1;j<=inst->is[i][0];j++)
      {
        data->precPendientes[inst->is[i][j]]++;
        if(data->precPendientes[inst->is[i][j]]==1)
        {
          data->candidatas[numCandidatas]=0;
          numCandidatas--;
        }
      }
      tiempoLibre=tiempoLibre+inst->d[i];
    }
  }
  if(estacionCompleta==1)
  {
    data->numAgrupaciones++;
    imprimirSolutionHastaAhora(depth,data);
    if(tiempoLibre<bestStationValue) //es mejor que la mejor hasta ahora?
    {
      //guardar mejor
      bestStationValue=tiempoLibre;
      numBestStation=depth-1; //resulta que en el código depth es el número de tarea que quiero colocar
      for(i=1;i<=numBestStation;i++) bestStation[i]=data->enConstruccion[i];
    }
  }
  return(0);
}

int enumeraUnaEstacion(instance* inst,int estacionEnCurso,int* pendientes,dataHoffmann *data)
{

  int i,ii,j,jj;
  int numCandidatas=0;
  int tiempoLibre;

  //primero inicializar la lista de tareas que puedo escoger, inicializar las precedencias pendientes
  //esto depende de las tareas que ya he realizado ahora (antes no)
  /* ANTES SÓLO FUNCIONA CON UNA ESTACION
  for(i=1;i<=inst->nt;i++)
  {
    if(inst->ip[i][0]==0)
    {
      numCandidatas++;
      data->candidatas[numCandidatas]=i;
    }
    data->precPendientes[i]=inst->ip[i][0];
  }
  */
  // AHORA, funciona si me preocupo de tener el número de precedentes pendientes en el vector pendientes
  for(i=1;i<=inst->nt;i++)
  {
    if(pendientes[i]==0)
    {
      numCandidatas++;
      data->candidatas[numCandidatas]=i;
    }
    data->precPendientes[i]=pendientes[i];
  }

  //segundo mirar para todas las tareas que puedo escoger
  //-> llama a la parte enumerativa
  bestStationValue=inst->c;
  tiempoLibre=inst->c;
  for(i=1;i<=numCandidatas;i++)
  {
    //añadir la tarea data->candidatas[i]
    //printf("he asignado la tarea %d\t",data->candidatas[i]);
    tiempoLibre=tiempoLibre-inst->d[data->candidatas[i]];
    data->enConstruccion[1]=data->candidatas[i];
    //printf("tiempoLibre %d\n",tiempoLibre);
    //actualizar precedencias
    for(j=1;j<=inst->is[data->candidatas[i]][0];j++)
    {
      data->precPendientes[inst->is[data->candidatas[i]][j]]--;
      if(data->precPendientes[inst->is[data->candidatas[i]][j]]==0)
      {
        numCandidatas++;
        data->candidatas[numCandidatas]=inst->is[data->candidatas[i]][j];
      }
    }
    /*
    printf("son data->candidatas: ");
    for(j=1;j<=numCandidatas;j++) printf("%d\t",data->candidatas[j]);
    printf("\n");
    */
    
    //llamar a la función para ver cuál es la segunda tarea que pongo
    recursion(inst,2,tiempoLibre,i+1,numCandidatas,data);

    //quitar la tarea data->candidatas[i]
    data->precPendientes[inst->is[data->candidatas[i]][j]]++;
    for(j=1;j<=inst->is[data->candidatas[i]][0];j++)
    {
      data->precPendientes[inst->is[data->candidatas[i]][j]]++;
      if(data->precPendientes[inst->is[data->candidatas[i]][j]]==1)
      {
        data->candidatas[numCandidatas]=0;
        numCandidatas--;
      }
    }
    tiempoLibre=tiempoLibre+inst->d[data->candidatas[i]];
    /*
    printf("he borrado la tarea %d\ttiempoLibre %d\n",data->candidatas[i],tiempoLibre);
    printf("son candidatas: ");
    for(j=1;j<=numCandidatas;j++) printf("%d\t",data->candidatas[j]);
    printf("\n");
    */
  }

  //voy a imprimir cuál es la mejor
  printf("la mejor tiene tiempo libre %d y %d tareas\t",bestStationValue,numBestStation);
  for(i=1;i<=numBestStation;i++)
  {
    printf("%d ",bestStation[i]);
  }
  printf("\n");
  //aquí tengo que actualizar la solución que estoy construyendo
  for(ii=1;ii<=numBestStation;ii++)
  {
    i=bestStation[ii];
    asignacionSolucion[i]=estacionEnCurso;
    assert(pendientes[i]==0);
    pendientes[i]=(-1);
    for(jj=1;jj<=inst->is[i][0];jj++)
    {
      j=inst->is[i][jj];
      pendientes[j]--;
    }
  }
  return(numBestStation);

}

int hoffmann(instance* inst,int numMaxAgrupaciones,solucion* s)
{
  int i;
  int tareasAsignadas=0;
  int estacionEnCurso=0;
  int precedenciasPendientes[inst->nt+1]; 
  dataHoffmann data;

  data.MAX_AGRUPACIONES=numMaxAgrupaciones;

  asignacionSolucion=generateIntVector(inst->nt+1); 
  bestStation=generateIntVector(inst->nt+1); //las tareas de la mejor hasta el momento
  data.candidatas=generateIntVector(inst->nt+1);
  data.precPendientes=generateIntVector(inst->nt+1);
  data.enConstruccion=generateIntVector(inst->nt+1);

  for(i=1;i<=inst->nt;i++)
  {
    precedenciasPendientes[i]=inst->ip[i][0];
    data.precPendientes[i]=inst->ip[i][0];
  }

  do
  {
    //genera todas las alternativas para la siguiente estación y selecciona la mejor
    estacionEnCurso++;
    tareasAsignadas=tareasAsignadas+enumeraUnaEstacion(inst,estacionEnCurso,&(precedenciasPendientes[0]),&data);
    //truco para salir tras una única llamada a enumerarUnaEstacion
    printf("estaciones construidas: %d con tareas asignadas: %d\n",estacionEnCurso,tareasAsignadas);
  }while(tareasAsignadas!=inst->nt);

  for(i=1;i<=inst->nt;i++)
  {
    printf("\tla tarea %d en estacion %d\n",i,asignacionSolucion[i]);
  }
  printf("numero de estacuiones: %d\n",estacionEnCurso);
  free(data.enConstruccion);
  free(asignacionSolucion);
  free(bestStation);
  free(data.candidatas);
  free(data.precPendientes);

  return(0);
}


