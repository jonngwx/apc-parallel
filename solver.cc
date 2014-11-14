#include <stdlib.h>
#include <stdio.h>
#include "euler.h"
#include "heat_eq.h"
#define PI 3.14159265358979

int main(int argc, const char * argv[]){

  Model * model;
  Integrator *integrator;
  
  int nx = 8;
  int ny = nx;
  double kappa = 1.;
  
  double dx = PI/(nx-1);
  double dt = dx*dx/4/kappa*.95;


  model = new Heat_Eq(kappa,nx,ny);
  integrator = new Euler(dt,*model);

  delete model;
  delete integrator;
  return 0;
}
