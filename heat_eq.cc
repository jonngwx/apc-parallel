#include "heat_eq.h"
#include <math.h>

Heat_Eq::Heat_Eq(double kappa, int nx, int ny): kappa(kappa), nx(nx), ny(ny)
{
  //create x and y and grid;
}

Heat_Eq::~Heat_Eq(){}

int Heat_Eq::rhs(double t, const double **x, double ** fx) const{
  // stuff
  return 0;
}

void bcx(double t, const double **x, double ** fx){}

void bcy(double t, const double **x, double ** fx){}
