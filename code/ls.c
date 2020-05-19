#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "salbp.h"

int calculateEarliest(int tarea,instance* inst,solucion* s)
{
  int i,ii;
  int estacion=1;

  //me tengo que quedar con el máximo
  for(ii=1;ii<=inst->ip[tarea][0];ii++)
  {
    i=inst->ip[tarea][ii];
    if(estacion<s->asignacion[i]) estacion=s->asignacion[i];
  }
  return(estacion);
}

int calculateLatest(int tarea,instance* inst,solucion* s)
{
  int i,ii;
  int estacion=s->numEstaciones;

  //me tengo que quedar con el mínimo
  for(ii=1;ii<=inst->is[tarea][0];ii++)
  {
    i=inst->is[tarea][ii];
    if(estacion>s->asignacion[i]) estacion=s->asignacion[i];
  }
  return(estacion);
}


int busquedalocal(instance* inst,solucion* s)
{
  int i,j,k;
  int mejora;

  //inicializar sumaD
  for(i=1;i<=s->numEstaciones;i++) s->sumaD[i]=0;
  for(i=1;i<=inst->nt;i++) s->sumaD[s->asignacion[i]] += inst->d[i];
  s->c=0;
  for(i=1;i<=s->numEstaciones;i++) 
  {
    if(s->c<s->sumaD[i]) s->c=s->sumaD[i];
  }

  //inicializar e
  for(i=1;i<=inst->nt;i++)
  {
    s->e[i]=calculateEarliest(i,inst,s);
  }
  //inicializar l
  for(i=1;i<=inst->nt;i++)
  {
    s->l[i]=calculateLatest(i,inst,s);
  }
  printf("\n\n==============================\n\n");
  for(i=1;i<=inst->nt;i++)
  {
    printf("tarea %d asignacion %d earliest %d latest %d\n",i,s->asignacion[i],s->e[i],s->l[i]);
  }
  printf("\n\n==============================\n\n");
  for(i=1;i<=s->numEstaciones;i++) printf("%d\t",s->sumaD[i]);
  printf("\n");


  //comprobar el vecindario de move
  do
  {
    mejora=0;
    for(i=1;i<=inst->nt;i++) //miro todas las tareas
    {
      if(s->sumaD[s->asignacion[i]]==s->c)  //(1) forma parte de una estación con carga máxima
      {
        printf("compruebo tarea %d (estacion original %d)\n",i,s->asignacion[i]);
        for(j=s->e[i];j<=s->l[i];j++) // compruebo todas las estaciones donde puede ir la tarea i
        {
          if(j!=s->asignacion[i]) //me aseguro que me muevo a una estación diferente
          {
          printf("\t moverla a estacion %d\n",j);
            if((s->sumaD[j]+inst->d[i])<s->c) //verifico que cabe sin "colapsar" la estación (puede mejorar c)
            { //hacer este movimiento mejora la solución en curso
              printf("\tmejora!!!\n");
              mejora=1;
              s->sumaD[s->asignacion[i]] -= inst->d[i];
              s->sumaD[j] += inst->d[i];
              //volver a calcular s->c
              s->c=0;
              for(k=1;k<=s->numEstaciones;k++)
              {
                if(s->c<s->sumaD[k])  s->c=s->sumaD[k];
              }
              //tengo que mover la tarea i a la estación j
              s->asignacion[i]=j;
              //actualizar los e y los j
              for(k=1;k<=inst->ip[i][0];k++)
              {
                calculateLatest(inst->ip[i][k],inst,s);
              }
              for(k=1;k<=inst->is[i][0];k++)
              {
                calculateEarliest(inst->is[i][k],inst,s);
              }
              break; //si ha habido un cambio, no quiero seguir probando combinaciones de la tarea i
            }
          }
        }
      }
    }
    for(i=1;i<=s->numEstaciones;i++) printf("%d\t",s->sumaD[i]);
    printf("\n");
  }while(mejora==1);

   //implementar swap
  do
  {
    mejora=0;
    for(i=1;i<inst->nt;i++)
    {
      for(j=i+1;j<=inst->nt;j++)
      {
        //asegurarme que son estaciones diferentes
        if(s->asignacion[i]==s->asignacion[j]) continue;
        //comprobación de criticidad
        if((s->sumaD[s->asignacion[i]]<s->c)&&(s->sumaD[s->asignacion[j]]<s->c)) continue;
        //comprobación de mejora
        if((s->sumaD[s->asignacion[i]]-inst->d[i]+inst->d[j])>=s->c) continue;
        if((s->sumaD[s->asignacion[j]]-inst->d[j]+inst->d[i])>=s->c) continue;
        //comprobación de earliest y latest
        if(s->e[i]>s->asignacion[j]) continue;
        if(s->l[i]<s->asignacion[j]) continue;
        if(s->e[j]>s->asignacion[i]) continue;
        if(s->l[j]<s->asignacion[i]) continue;
        //la tarea i no puede tener una relación de predecencia con la tarea j
        if(inst->p[i][j]==1) continue;
        //la tarea j no puede tener una relación de precedencias con la tarea i
        if(inst->p[j][i]==1) continue;
        //SI LLEGA AQUÍ, ES UNA MEJORA
        printf("comprueba %d (estacion %d dura %d) con %d (estacion %d dura %d) mejora!\n",i,s->asignacion[i],inst->d[i],j,s->asignacion[j],inst->d[j]);
        mejora=1;
        //cambiar las asignaciones de tiempo a las estaciones
        s->sumaD[s->asignacion[i]]=s->sumaD[s->asignacion[i]]-inst->d[i]+inst->d[j];
        s->sumaD[s->asignacion[j]]=s->sumaD[s->asignacion[j]]+inst->d[i]-inst->d[j];
        //volver a calcular s->c
        s->c=0;
        for(k=1;k<=s->numEstaciones;k++)
        {
          if(s->c<s->sumaD[k])  s->c=s->sumaD[k];
        }
        //cambiar las asignaciones
        k=s->asignacion[i];
        s->asignacion[i]=s->asignacion[j];
        s->asignacion[j]=k;
        //cambiar los earliest y los latest (por el cambio de la tarea j)
        for(k=1;k<=inst->ip[i][0];k++)
        {
          calculateLatest(inst->ip[i][k],inst,s);
        }
        for(k=1;k<=inst->is[i][0];k++)
        {
          calculateEarliest(inst->is[i][k],inst,s);
        }
        //cambiar los earliest y los latest (por el cambio de la tarea j)
        for(k=1;k<=inst->ip[j][0];k++)
        {
          calculateLatest(inst->ip[j][k],inst,s);
        }
        for(k=1;k<=inst->is[j][0];k++)
        {
          calculateEarliest(inst->is[j][k],inst,s);
        }
      }
    }
  }while(mejora==1);
  

  printf("\n\n= FINAL FINALÍSIMO =============================\n\n");
  for(i=1;i<=s->numEstaciones;i++) printf("%d\t",s->sumaD[i]);
  printf("\n");
  //valdria la pena check de que las s->sumaD se hacen correctamente.
  return(0);
}
