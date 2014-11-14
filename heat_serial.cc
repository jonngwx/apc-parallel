#include <stdlib.h>
#include <stdio.h>
#include "euler.h"
#include "utils.h"
#include "heat_eq.h"
#include <time.h>
#include <string.h>
#define PI 3.14159265358979

int main(int argc, const char * argv[]){

  Model * model;
  Integrator *integrator;
  
  if (argc != 2){
    printf("Requires argument <nx> \n");
    return 0;
  }

  int nx = atoi(argv[1]);
  int ny = nx;

  if (nx < 2){
    printf("grid is too small. requires nx >= 3 for nontrivial solution");
    return 0;
  }
  
  clock_t time;
  time = clock();

  const double kappa = 1.;
  
  const double dx = PI/(nx-1);
  const double dt = dx*dx/4/kappa*.95;

  const double tmax = .5*PI*PI/kappa;
  double t = 0.;
  model = new Heat_Eq(kappa,nx,ny);
  integrator = new Euler(dt,*model);


  while (t<tmax){
    integrator->Step(t, (*model).get_grid());
    t = t+dt;
  }
  time = clock() - time;
  //printf("%1.8f\n",((float)time)/CLOCKS_PER_SEC);

  char filename[80];
  strcpy(filename,"heat_out_");
  strcat(filename,argv[1]);
  double Tave = average((*model).get_grid(), nx,ny);
  output_2d((*model).get_grid(),nx,ny,filename);
  printf("%1.8f\n%10.8f\n",Tave, ((float)time)/CLOCKS_PER_SEC);
  delete model;
  delete integrator;
  return 0;
}
