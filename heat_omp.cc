#include <stdlib.h>
#include <stdio.h>
#include "euler.h"
#include "utils.h"
#include "heat_eq.h"
#include <time.h>
#include <string.h>
#include <omp.h>
#define PI 3.14159265358979

int main(int argc, const char * argv[]){

  Model * model;
  Integrator *integrator;
  
  if (argc != 3){
    printf("Requires argument <nx> <nthreads>\n");
    return 0;
  }

  int nx = atoi(argv[1]);
  int ny = nx;
  int nthreads = atoi(argv[2]);

  //input checking
  if (nx < 2){
    printf("grid is too small. requires nx >= 3 for nontrivial solution");
    return 0;
  }
  if (nthreads < 1){
    printf("needs more than one thread to start...");
    return 0;
  }
  // omp_set_dynamic(0);
  omp_set_num_threads(nthreads);
  double time;
  time = omp_get_wtime();
  //  printf("%1d, %1d\n",nthreads,omp_get_num_threads());
  //  return 0;
  const double kappa = 1.;
  
  const double dx = PI/(nx-1);
  const double dt = dx*dx/4/kappa*.95; // stability condition.

  const double tmax = .5*PI*PI/kappa;
  double t = 0.;
  model = new Heat_Eq(kappa,nx,ny);
  integrator = new Euler(dt,*model);


  while (t<tmax){
    integrator->Step(t, (*model).get_grid());
    t = t+dt;
  }
  time = omp_get_wtime() - time;

  char filename[80];
  strcpy(filename,"heat_out_omp_");
  strcat(filename,argv[1]);
  double Tave;
  Tave = average((*model).get_grid(), nx,ny);
  output_2d((*model).get_grid(),nx,ny,filename);
  printf("%1.8f, %10.8f\n",Tave, time);
  delete model;
  delete integrator;
  return 0;
}
