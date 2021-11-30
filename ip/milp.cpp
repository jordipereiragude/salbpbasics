#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <ilcplex/ilocplex.h>
#include <iostream>

ILOSTLBEGIN

IloNumArray RHS;

extern int nt;
extern int ne;
extern int* d;
extern int** p;

int solveMILP(int t_limit) {
  double init=clock() / CLOCKS_PER_SEC;

  IloEnv env;
  IloModel model(env);
  IloCplex Solver(model);

  IloNumVarArray x_ij(env,nt*ne,0.0,1.0,ILOINT); //asignación de tareas
  IloNumVar c(env,0,IloInfinity,ILOFLOAT); //tiempo de ciclo

  model.add(IloMinimize(env, c));	// minimizar tiempo de ciclo
  for(auto i=0;i<nt;i++) //asignar todas las tareas
  {
    IloExpr Expr(env);
    for(auto j=0;j<ne;j++)
    {
      Expr += x_ij[i*ne+j];
    }
    model.add(Expr == 1);
    Expr.end();
  }
  for(auto j=0;j<ne;j++) //calcular c
  {
    IloExpr Expr(env);
    for(auto i=0;i<nt;i++)
    {
      Expr += x_ij[i*ne+j]*d[i+1]; //cuidado por cambio de numeración
    }
    model.add(Expr <= c);
    Expr.end();
  }
  for(auto i=0;i<nt;i++) //precedencias
  {
    for(auto ip=0;ip<nt;ip++)
    {
      if(p[i+1][ip+1]==1) //son precedentes
      {
        IloExpr Expr(env);
        for(auto j=0;j<ne;j++)
        {
          Expr += x_ij[i*ne+j]-x_ij[ip*ne+j]; //considero lado derecho con negativo en lado izquierdo
        }
        model.add(Expr <= 0);
        Expr.end();
      }
    }
  }
  Solver.setOut(env.getNullStream()); //elimina output por pantalla
  Solver.setParam(IloCplex::Threads,1); //usa sólo un hilo de ejecución
  Solver.setParam(IloCplex::TiLim,(double)t_limit); //límite de tiempo
  Solver.solve();
  //cuidado porque si Status no es Optimal o Feasible, esta línea devuelve un error
  std::cout << "status: " << Solver.getStatus() << " obj: " << "status: " << Solver.getStatus() << " obj: " << Solver.getObjValue() << std::endl;
  env.end();
  return(0);
}


