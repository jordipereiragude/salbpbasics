#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <vector>

#include "salbp.h"

void salbp::set_direction(){
  int i,j;

  if(run_forward>(-1)) //forward or backward
  {
    printf("direction %d ",run_forward);
    if(run_forward==0) 
    {
      reverse_pred();
      close_pred();
      find_successors();
      compute_descending_order();
    }
  }
  else
  {
	  // Determine whether to run in forward or reverse
	  for (int j = 1; j <= n_tasks; ++j)
	  {
		  double ftime = t[j];
		  double rtime = t[j];
		  for (int i = 1; i <= n_tasks; ++i)
		  {
			  if (closed_predecessor_matrix[i][j]) ftime += t[i];       // If task i precedes task j
			  if (closed_predecessor_matrix[j][i]) rtime += t[i];       // If task j precedes task i
		  }

		  earliest[j] = ceil(ftime/LB);
		  latest[j] = ceil(rtime/LB);
	  }
	  unsigned long long int f = 1;
	  unsigned long long int r = 1;
	  for (int m = 1; m <= 5; ++m)
	  {
		  int fcount = 0;      //changed by AS 2013/06/08
		  int rcount = 0;		//changed by AS 2013/06/08
		  for (int j = 1; j <= n_tasks; ++j)
		  {
			  if (earliest[j] <= m) ++fcount;
			  if (latest[j] <= m) ++rcount;
		  }

		  f *= fcount;
		  r *= rcount;
	  }
	  if (r < f)
	  {
		  printf("direction 0\t");
      reverse_pred();
      close_pred();
      find_successors();
      compute_descending_order();
	  }
	  else printf("direction 1\t");
  }
  return;
}

void salbp::report_time(){
  printf(" %5.2f ",(double)(clock()-stats.start_time)/ CLOCKS_PER_SEC);
}

void salbp::report_stats() {
    printf("optimality %d totalTime: %5.2f mmh: %d hoffman_cpu: %5.2f first_phase_UB: %d first_phase_cpu: %5.2f first_phase_nodes: %d first_phase_explored_nodes: %d second_phase_UB: %d second_phase_cpu: %5.2f second_phase_nodes: %d second_phase_explored_nodes: %d final_upper_bound: %d final_lower_bound: %d\n",
            verified_optimality,(double)(clock()-stats.start_time)/ CLOCKS_PER_SEC,
            stats.best_mmh,stats.hoffman_cpu,
            stats.best_first_phase,stats.first_phase_cpu,stats.first_phase_nodes,stats.first_phase_explored_nodes,
            stats.best_second_phase,stats.second_phase_cpu,stats.second_phase_nodes,stats.second_phase_explored_nodes,
            getUB(),get_final_LB());
}

void salbp::set_start_time() {
  stats.start_time = clock();
}

int salbp::get_verified_optimality() {
  return(verified_optimality);
}

void salbp::set_to_optimality() {
  verified_optimality=1;
  max_full_loads_bbr=INT_MAX;
  return;
}

void salbp::setSecondPhaseUB() {
  stats.best_second_phase=UB;
  return;
}

int salbp::getLB() {
  return(LB);
}

void salbp::setLB(int val) {
  LB=val;
}

int salbp::getUB() {
  return(UB);
}

void salbp::usage (char *prog) {
/*
  returns the list of options
*/
  fprintf (stderr, "Usage: %s\n", prog);
  fprintf (stderr, "    -i: useIP 0 = no (default), 1 = yes\n");
  fprintf (stderr, "    -f: filename\n");
  fprintf (stderr, "    -m: Max num Vertices\n");
  fprintf (stderr, "    -t: CPU time limit\n");
  fprintf (stderr, "    -d: Set direction (-1 = auto (default), 0 = reverse, 1 = forward)\n");
  fprintf (stderr, "    -l: max_full_loads_bbr\n");
  fprintf (stderr, "    -h: max_full_loads_hoffman\n");
	fprintf (stderr, "    -s: num_stations\n");
  fprintf (stderr, "    -y; file type 0 = traditional (default), 1 = otto et al.\n");
  fprintf (stderr, "    -p; priority 0 state by largest sum of task times among assigned tasks, 1 selecting state by smallest idle time in stations (default)\n");
  exit (1);
}

void salbp::parseargs(int ac, char **av)
/*
  parses the input line
*/
{
	int c, cnt;

	cnt = 0;
  run_forward=(-1);
  max_full_loads_bbr=10000; //tentativo mirar
  max_full_loads_hoffman=5000;
  max_num_vertices=1000000000;
  cpu_limit=3600;
  UB=BIG_INT;
  LB=0;
  m=0;
  filetype=0;
  useIP=0;
  priorityRule=1;
	while (++cnt < ac && av[cnt][0] == '-')
	{
		c = av[cnt][1];
		switch (c)
		{
			case 's':
				m = atoi(av[++cnt]);
				break;
			case 't':
				cpu_limit = atoi(av[++cnt]);
				break;
			case 'd':
				run_forward = atoi(av[++cnt]);
				break;
			case 'l':
				max_full_loads_bbr = atoi(av[++cnt]);
				break;
      case 'h':
  			max_full_loads_hoffman = atoi(av[++cnt]);
  			break;
			case 'f':
				prob_file = av[++cnt];
				break;
			case 'm':
				max_num_vertices = atoi(av[++cnt]);
				break;
			case 'y':
				filetype = atoi(av[++cnt]);
				break;
      case 'i':
        useIP=atoi(av[++cnt]);
        break;
      case 'p':
        priorityRule=atoi(av[++cnt]);
        break;
			default:
				usage(*av);
				break;
		}
	}
	if (prob_file==NULL)
  {
    usage (*av);
    exit(0);
  }
  if(run_forward>1)
  {
    usage (*av);
    exit(0);
  }
  if(run_forward<(-1))
  {
    usage (*av);
    exit(0);
  }
	if(m==0)
  {
    usage(*av);
    exit(0);
  }
	if (cnt > ac)
  {
    usage (*av);
    exit(0);
  }
	if (cnt < ac)
  {
    usage (*av);
    exit(0);
  }
  if(useIP>1)
  {
    usage (*av);
    exit(0);
  }
  if(priorityRule>1)
  {
    usage (*av);
    exit(0);
  }
  if(priorityRule<0)
  {
    usage (*av);
    exit(0);
  }
}

//genera el grafo denso de precedencias
void salbp::close_pred()
/*
   1. This function creates the closed predecessor matrix from a predecessor matrix.
   2. The predecessor matrix must be acyclic.
   3. This function assumes that the tasks have been sorted such that (i,j) is an
	  edge implies that i < j.
   4. predecessor_matrix[i][j] = 1 if task i precedes task j, 0 o.w.
   5. closed_predecessor_matrix[i][j] = 1 if there is a directed path from i to j in the graph.
   6. Written 2/28/06.
*/
{

  int      i, j, k;
  // Copy predecessor_matrix into closed_predecessor_matrix, check that the entries
  // are either 0 or 1, and check that the tasks are topologically sorted.
  for(i = 1; i <= n_tasks; i++)
  {
	  for(j = 1; j <= n_tasks; j++)
    {
      assert((predecessor_matrix[i][j] == 0) || (predecessor_matrix[i][j] == 1));
      assert((predecessor_matrix[i][j] == 0) || (i < j));
      closed_predecessor_matrix[i][j] = predecessor_matrix[i][j];
    }
  }
  // Compute the closure.
  for(k = 2; k <= n_tasks; k++)
  {
    for(j = 1; j < k; j++)
    {
      if(closed_predecessor_matrix[j][k] == 1)
      {
        for(i = 1; i < k; i++)
        {
          if(closed_predecessor_matrix[i][j] == 1) closed_predecessor_matrix[i][k] = 1;
        }
      }
    }
  }
}

void salbp::find_successors()
/*
  sparsifies the predecessor table
*/
{
  int      i, j, k1, k2;

  for(i = 1; i <= n_tasks; i++)
  {
	  k1 = 0;
	  k2 = 0;
	  for(j = 1; j <= n_tasks; j++)
    {
      if(predecessor_matrix[j][i] == 1) predecessors[i][++k1] = j;
      if(predecessor_matrix[i][j] == 1) successors[i][++k2] = j;
	  }
	  predecessors[i][0] = k1;
	  successors[i][0] = k2;
  }
}

void salbp::reverse_pred()
/*
   1. This function reverses the predecessor matrix and the t values.
   2. Written 3/2/06.
*/
{
  int i, j, temp;

  for(i = 1; i <= n_tasks; i++)
  {
    for(j = 1; j <= n_tasks - i + 1; j++)
    {
	    temp = predecessor_matrix[i][j];
		  predecessor_matrix[i][j] = predecessor_matrix[n_tasks-j+1][n_tasks-i+1];
      predecessor_matrix[n_tasks-j+1][n_tasks-i+1] = temp;
	  }
  }
  i = 1;
  j = n_tasks;
  while(i < j)
  {
    temp = t[i];
    t[i] = t[j];
    t[j] = temp;
    i++;
    j--;
  }
}

void salbp::compute_potentially_dominated()
/*
  Jackson dominance rule preprocessing
   1. This function determines which tasks potentially dominate one another.
   2. It uses the strengthened Jackson dominance rule (Scholl and Klein, 1997).
   3. Task i potentially dominates task j if the following conditions are satisfied:
	  a. i and j are not related by a precedence relationship.
	  b. t(i) >= t(j).
	  c. The successors of i include the successors of j.
	  d. If t(i) = t(j) and i and j have the same set of successors, then the task with the smaller task number
		 dominates the other one.
   4. potentially_dominates[i][j] = 1 if task i potentially dominates task j.
   5. This function uses closed_predecessor_matrix, so it must be computed before this function is called.
   6. Written 3/21/06.
   */
{
  int i, j, k, kk, stop, superset;
  //clean everything
  for(i = 1; i <= n_tasks; i++) potentially_dominated[i][0]=0;
	for(i = 1; i <= n_tasks; i++)
	{
		for(j = 1; j <= n_tasks; j++)
		{
			// Check that the entries are either 0 or 1
			assert((closed_predecessor_matrix[i][j] == 0) || (closed_predecessor_matrix[i][j] == 1));
			// Check that the tasks are topologically sorted.
			assert((closed_predecessor_matrix[i][j] == 0) || (i < j));
			if ((i != j) && (t[i] >= t[j]) && (closed_predecessor_matrix[i][j] == 0) && (closed_predecessor_matrix[j][i] == 0))
			{
				// Determine if the successors of i contain all the successors of j.
				superset = 1;
				stop = successors[j][0];
				for(kk = 1; kk <= stop; kk++)
				{
					k = successors[j][kk];
					if(closed_predecessor_matrix[i][k] != 1) superset = 0;
				}
				// If superset = 1, determine if the two sets of successors are the same.
				if(superset == 1)
				{
					if(successors[i][0] == successors[j][0]) superset = 2;
				}
				if(superset >= 1)
				{
					if ((t[i] > t[j]) || ((t[i] == t[j]) && ((i < j) || (superset == 1))))
					{
            potentially_dominated[j][0]++;
						potentially_dominated[j][potentially_dominated[j][0]] = i;
					}
				}
			}
		}
	}
}

void salbp::compute_positional_weights()
/*
   1. This function computes the number of predecessors, number of successors, and the
	  positional weights from the closed predecessor matrix.
   2. positional_weight[i] = t[i] + sum(t[j]: j is a successor of i).
   3. Written 2/28/06.
   (used by Hoffmann)
*/
{
	int      i, j, sum;

	for(i = 1; i <= n_tasks; i++)
	{
		positional_weight[i] = t[i];
	}
	for(i = 1; i <= n_tasks; i++)
	{
		sum = 0;
		for(j = i+1; j <= n_tasks; j++)
		{
			if(closed_predecessor_matrix[i][j] == 1)
			{
				sum += t[j];
			}
		}
		positional_weight[i] += sum;
	}
}

void salbp::read_file()
/*
  Multipurpose initialization function:
    1.- reads the filename
    2.- Allocates memory
    3.- Calls closed predecessor graph
    4.- Obtains sparse list of predecessors
    5.- Calculates a simple lower and upper bound
*/
{
  FILE* in;
  char* line;
  int i,j,d1,d2,out;

  c_maxLB4=0; //start things for memory later
  printf("%s stations %d max_full_loads_bbr %d direction %d timeLimit %d ",prob_file,m,max_full_loads_bbr,run_forward,cpu_limit);
  fflush(stdout);
  if(filetype==0)
  {
    in=fopen(prob_file,"rt");
	  fscanf(in,"%d\n",&n_tasks);
    allocate_memory();
	  //printf("t: %d\n",n_tasks);
	  //fflush(stdout);
	  t[0] = n_tasks;
    tMax=0;
    tSum=0;
	  for (i = 1; i <= n_tasks; i++)
	  {
		  for (j = 1; j <= n_tasks; j++) predecessor_matrix[i][j] = 0;
  	}
  	for(i=1;i<=n_tasks;i++)
	  {
		  fscanf(in,"%d\n",&t[i]);
		  //printf("t %d %d\n",i,t[i]);
		  //fflush(stdout);
      tSum += t[i];
      tMax=MAX(tMax,t[i]);
	  }
	  out=0;
	  do
	  {
		  fscanf(in,"%d,%d\n",&d1,&d2);
		  //printf("precedence relations: %d %d\n",d1,d2);
		  //fflush(stdout);
		  if(d1==(-1)) out=1;
		  else predecessor_matrix[d1][d2] = 1;
		  if(feof(in)) out=1;
	  }while(out==0);
  	fclose(in);
  }
  else
  {
    line=(char*)malloc(256*sizeof(char));
    in=fopen(prob_file,"rt");
    while(!feof(in))
    {
      fscanf(in,"%s",line);
  		//n
      if((line[0]=='t')&&(line[1]=='a')&&(line[2]=='s')&&(line[3]=='k')&&(line[4]=='s')&&(line[5]=='>'))
		  {
			  //printf("N\n");
			  fscanf(in,"%s",line);
        n_tasks=atoi(line);
        //printf("n_tasks: %d\n",n_tasks);
        allocate_memory();
	      t[0] = n_tasks;
        tMax=0;
        tSum=0;
      }
	  	if((line[0]=='t')&&(line[1]=='i')&&(line[2]=='m')&&(line[3]=='e')&&(line[4]=='>'))
		  {
			  //printf("TC\n");
			  //count=0;
			  fscanf(in,"%s",line);
			  //instancia->tc=atoi(linea); //irrelevant in this problem
		  }
      //task times
		  if((line[0]=='t')&&(line[1]=='i')&&(line[2]=='m')&&(line[3]=='e')&&(line[4]=='s')&&(line[5]=='>'))
		  {
			  //printf("task times\n");
			  for(i=1;i<=n_tasks;i++)
			  {
				  fscanf(in,"%d %d",&out,&j);
				  if(i!=out)
				  {
					  printf("gazapo lectura duraciones\n");
					  exit(0);
				  }
				  t[i]=j;
				  //printf("i: %d t[i]: %d\n",i,t[i]);
          tSum += t[i];
          tMax=MAX(tMax,t[i]);
			  }
			  //printf("\n");
		  }
      //prec
		  if((line[0]=='r')&&(line[1]=='e')&&(line[2]=='l')&&(line[3]=='a')&&(line[4]=='t')&&(line[5]=='i'))
		  {
			  fscanf(in,"%s",line);
			  do
			  {
				  sscanf(line,"%d,%d",&d1,&d2);
          predecessor_matrix[d1][d2] = 1;
          //printf("prec: %d %d\n",d1,d2);
				  fscanf(in,"%s",line);
			  }while(line[0]!='<');
		  }
    }
    fclose(in);
    free(line);
  }
  close_pred();
  //upper and lower bounds according to Klein, Scholl (1996) EJOR vol.91 pp. 367-385
  i=2*tSum/m;
  UB=MAX(tMax,i);
  if((tSum%m)==0) i=tSum/m;
  else i=tSum/m+1;
  LB=MAX(tMax,i);
  find_successors();
  compute_descending_order();
}

void salbp::read_file_Otto()
/*
  Multipurpose initialization function:
    1.- reads the filename (Otto style)
    2.- Allocates memory
    3.- Calls closed predecessor graph
    4.- Obtains sparse list of predecessors
    5.- Calculates a simple lower and upper bound
*/
{
  FILE* in;
  int i,j,d1,d2,out;

  c_maxLB4=0; //start things for memory later
  printf("%s stations %d ",prob_file,m);
  fflush(stdout);
  in=fopen(prob_file,"rt");
	fscanf(in,"%d\n",&n_tasks);
  allocate_memory();
	//printf("t: %d\n",n_tasks);
	//fflush(stdout);
	t[0] = n_tasks;
  tMax=0;
  tSum=0;
	for (i = 1; i <= n_tasks; i++)
	{
		for (j = 1; j <= n_tasks; j++) predecessor_matrix[i][j] = 0;
	}

	for(i=1;i<=n_tasks;i++)
	{
		fscanf(in,"%d\n",&t[i]);
		//printf("t %d %d\n",i,t[i]);
		//fflush(stdout);
    tSum += t[i];
    tMax=MAX(tMax,t[i]);
	}
	out=0;
	do
	{
		fscanf(in,"%d,%d\n",&d1,&d2);
		//printf("precedence relations: %d %d\n",d1,d2);
		//fflush(stdout);
		if(d1==(-1)) out=1;
		else predecessor_matrix[d1][d2] = 1;
		if(feof(in)) out=1;
	}while(out==0);
	fclose(in);
  close_pred();
  //upper and lower bounds according to Klein, Scholl (1996) EJOR vol.91 pp. 367-385
  i=2*tSum/m;
  UB=MAX(tMax,i);
  if((tSum%m)==0) i=tSum/m;
  else i=tSum/m+1;
  LB=MAX(tMax,i);
  find_successors();
  compute_descending_order();
}

void salbp::compute_descending_order()
/*
   1. This function computes descending_order.
   2. descending_order[k] = the task with the kth largest processing time.
   3. Written 5/18/06.
*/
{
	int      i, c;

  // Sort the tasks in order of decreasing processing time.
	for(i = 1; i <= n_tasks; i++) descending_order[i] = i;
  do {
    c=0;
    for(i=1;i<n_tasks;i++)
    {
      if(t[descending_order[i]]<t[descending_order[i+1]])
      {
        c=descending_order[i];
        descending_order[i]=descending_order[i+1];
        descending_order[i+1]=c;
      }
    }
  } while(c!=0);
	for(i = 1; i <= n_tasks - 1; i++) assert(t[descending_order[i]] >= t[descending_order[i+1]]);
}

void salbp::allocate_memory()
/*
  This code mallocs all necessary (non-c++ related) memory requirements
*/
{
  int i;

  MALLOC(t,n_tasks+1,int);
  MALLOC(predecessor_matrix, n_tasks+1, char *);
	for (i = 1; i <= n_tasks; i++)
	{
		MALLOC(predecessor_matrix[i], n_tasks+1, char);
  }
  MALLOC(closed_predecessor_matrix, n_tasks+1, char *);
  for(i = 1; i <= n_tasks; i++)
  {
    MALLOC(closed_predecessor_matrix[i], n_tasks+1, char);
  }
  MALLOC(predecessors, n_tasks+1, int *);
  for(i = 1; i <= n_tasks; i++) MALLOC(predecessors[i], n_tasks+1, int);
  MALLOC(successors, n_tasks+1, int *);
  for(i = 1; i <= n_tasks; i++) MALLOC(successors[i], n_tasks+1, int);

  MALLOC(degrees, n_tasks+1, char);
  MALLOC(best_degrees,n_tasks+1,char);
  MALLOC(best_tasks, n_tasks+1, short);
  MALLOC(eligible, n_tasks+1, short);
  MALLOC(tasks, n_tasks+1, short);
  MALLOC(stations, n_tasks+1, short);
  MALLOC(start_station, n_tasks+2, int);

  MALLOC(positional_weight, n_tasks+1, int);
  MALLOC(descending_order,n_tasks+1,int);
  MALLOC(potentially_dominated, n_tasks+1, int *);
  for(i = 1; i <= n_tasks; i++) MALLOC(potentially_dominated[i], n_tasks+1, int);

  MALLOC(pending_task_times,n_tasks+1,int);
  MALLOC(earliest,n_tasks+1,int);
  MALLOC(latest,n_tasks+1,int);
}

void salbp::free_memory()
/*
  Frees all memory allocated with allocate_memory
*/
{
  int i;

  free(t);
  for(int i = 1; i <= n_tasks; i++) free(predecessor_matrix[i]);
  free(predecessor_matrix);
  for(int i = 1; i <= n_tasks; i++) free(closed_predecessor_matrix[i]);
  free(closed_predecessor_matrix);
  for(i = 1; i <= n_tasks; i++) free(predecessors[i]);
  free(predecessors);
  for(i = 1; i <= n_tasks; i++) free(successors[i]);
  free(successors);
  free(descending_order);
  for(i = 1; i <= n_tasks; i++) free(potentially_dominated[i]);
  free(potentially_dominated);
  free(degrees);
  free(best_degrees);
  free(best_tasks);
  free(eligible);
  free(tasks);
  free(stations);
  free(start_station);
  free(positional_weight);
  if(c_maxLB4>0)
  {
    for(i=c_minLB4;i<=c_maxLB4;i++) free(lb4_orden_tails[i]);
    free(lb4_orden_tails);
    for(i=c_minLB4;i<=c_maxLB4;i++) free(lb4_values_tails[i]);
    free(lb4_values_tails);
  }
  free(lb4_orden_heads);
  free(lb4_values_heads);
  free(earliest);
  free(latest);
}
