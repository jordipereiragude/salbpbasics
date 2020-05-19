#include "salbp.h"
#include "utils.h"

int readFile(instance* inst,char* nombre)
{
  FILE* in;
  int i,j,k;
  int cambio;

  printf("nombre del archivo: %s\n",nombre);
  in=fopen(nombre,"rt");
  fscanf(in,"%d\n",&(inst->nt));
  printf("numero de tareas: %d\n",inst->nt);
  //inicio reserva memoria
  inst->d=generateIntVector(inst->nt+1);
  inst->p=generateIntMatrix(inst->nt+1,inst->nt+1);
  inst->ip=generateIntMatrix(inst->nt+1,inst->nt+1);
  inst->is=generateIntMatrix(inst->nt+1,inst->nt+1);
  //fin reserva memoria
  //limpieza
  for(i=1;i<=inst->nt;i++)
  {
    for(j=1;j<=inst->nt;j++)
    {
      inst->p[i][j]=0;
    }
  }
  for(i=1;i<=inst->nt;i++) inst->ip[i][0]=0;
  for(i=1;i<=inst->nt;i++) inst->is[i][0]=0;
  //leer duraciones
  for(i=1;i<=inst->nt;i++)
  {
    fscanf(in,"%d\n",&(inst->d[i]));
  }
  for(i=1;i<=inst->nt;i++) printf("duración tarea %d es %d\n",i,inst->d[i]);
  for(;;)
  {
    fscanf(in,"%d,%d\n",&i,&j);
    if(i==(-1)) break;
    printf("la tarea %d es predecesora de la tarea %d\n",i,j);
    inst->p[i][j]=1;
    inst->is[i][0]=inst->is[i][0]+1;
    inst->is[i][inst->is[i][0]]=j; 
    inst->ip[j][0]=inst->ip[j][0]+1;
    inst->ip[j][inst->ip[j][0]]=i; 
  }
  fclose(in);
  for(i=1;i<=inst->nt;i++)
  {
    printf("tarea %d con duracion %d ip %d e is %d\n",i,inst->d[i],inst->ip[i][0],inst->is[i][0]);
    for(j=1;j<=inst->ip[i][0];j++) printf("\t%d",inst->ip[i][j]);
    printf("\n");
    for(j=1;j<=inst->is[i][0];j++) printf("\t%d",inst->is[i][j]);
    printf("\n");
  }
  //calculo de todas las predecesoras (construir matriz inst->p
  do
  {
    cambio=0;
    for(i=1;i<=inst->nt;i++)
    {
      for(j=1;j<=inst->nt;j++)
      {
        if((i!=j)&&(inst->p[i][j]==0))
        {
          for(k=1;k<=inst->nt;k++)
          {
            if((i!=k)&&(j!=k)&&(inst->p[i][k]==1)&&(inst->p[k][j]==1))
            {
              inst->p[i][j]=1;
              cambio=1;
            }
          }
        }
      }
    }
  }while(cambio==1);
  return(0);
}
