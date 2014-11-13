#include "heat_eq.h"
#include <math.h>
#include "utils.h"

Heat_Eq::Heat_Eq(double kappa, int nx, int ny): kappa(kappa), nx(nx), ny(ny)
{
  x = new double[nx];
  y = new double[ny];
  T = new double*[nx];
  // domain is zero to pi in both dirs
  const double pi = 3.14159265358979;
  for (int i = 0; i < nx; i++){
    
    T[i] = new double[ny];
  }//create x and y and grid;
}

Heat_Eq::~Heat_Eq(){
  delete [] x;
  delete [] y;
  for (int i = 0; i < nx; i++){
    delete [] T[i];
  }
  delete [] T;
}

int Heat_Eq::rhs(double t, const double **T, double ** fx) const{
  for (int i = 1; i < (nx - 1); i++){ // use 1,n-1 due to boundary terms
    for (int j = 1; j < (ny - 1); j++){
      fx[i][j] = kappa * nabla_squared(T,x,y,i,j);
    }
  }// boundary conditions
  this->bcx(t, T, fx);
  this->bcy(t, T, fx);
  return 0;
}

void Heat_Eq::bcx(double t, const double **x, double ** fx) const{}

void Heat_Eq::bcy(double t, const double **x, double ** fx) const{}
