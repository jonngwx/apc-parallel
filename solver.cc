#include <stdlib.h>
#include <stdio.h>
#include "euler.h"
#include "utils.h"
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

  const double tmax = .5*PI*PI/kappa;
  double t = 0.;
  model = new Heat_Eq(kappa,nx,ny);
  integrator = new Euler(dt,*model);


  while (t<tmax){
    integrator->Step(t, (*model).get_grid());
    t = t+dt;
  }
  output((*model).get_grid(),nx,ny);
  delete model;
  delete integrator;
  return 0;
}
