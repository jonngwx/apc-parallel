#include "heat_eq.h"
#include <math.h>
#include <stdio.h>
#include "utils.h"

Heat_Eq::Heat_Eq(double kappa, int nx, int ny): nx(nx), ny(ny), kappa(kappa)
{
  x = new double[nx];
  y = new double[ny];
  T = new double*[nx];
  // domain is zero to pi in both dirs
  const double pi = 3.14159265358979;
  const double dx = pi/(nx-1);
  const double dy = pi/(ny-1);
  for (int i = 0; i < nx; i++){
    x[i] = 0 + i*dx;
    T[i] = new double[ny]();
  }//create x and y and grid;
  for (int j = 0; j < ny; j++){
    y[j] = 0 + j*dy;
  }
  bcx_init();
  bcy_init();
}

Heat_Eq::~Heat_Eq(){
  delete [] x;
  delete [] y;
  for (int i = 0; i < nx; i++){
    delete [] T[i];
  }
  delete [] T;
}

int Heat_Eq::rhs(double t, const double * const *T, double ** fx) const{
#pragma omp parallel for
  for (int i = 1; i < (nx - 1); i++){ // use 1,n-1 due to boundary terms
    for (int j = 1; j < (ny - 1); j++){
      fx[i][j] = kappa * nabla_squared(T,x,y,i,j);
    }
  }// boundary conditions
  bcx(t, T, fx);
  bcy(t, T, fx);

  return 0;
}

// boundary conditions in x are periodic
// boundary conditions in y are not T(x,0) = cos^2 x
// T(x,pi) = sin^2 x
void Heat_Eq::bcx(double t, const double * const *T, double ** fx) const{
#pragma omp parallel for
  for(int j = 1; j < ny -1; j++){
    fx[0][j] = kappa * (d_squared_y(T,y,0,j) + (T[nx-2][j] + T[1][j] -2 * T[0][j])/(x[1]-x[0])/(x[1]-x[0]));
    fx[nx-1][j] = fx[0][j];
  }
  
}

void Heat_Eq::bcy(double t, const double * const *T, double ** fx) const{
#pragma omp parallel for
  for (int i = 0; i < nx; i++){
    fx[i][0] = 0;//cos(x[i]) * cos(x[i]);
    fx[i][ny-1] =0;// sin(x[i]) * sin(x[i]);
  }
}

void Heat_Eq::bcx_init() const{
#pragma omp parallel for
  for (int j = 1; j < ny-1; j++){
    T[0][j] = T[nx-1][j];
  }
}

void Heat_Eq::bcy_init() const{
#pragma omp parallel for
  for (int i = 0; i < nx; i++){
    T[i][0] = cos(x[i]) * cos(x[i]);
    T[i][ny-1] = sin(x[i]) * sin(x[i]);
  }

}

int Heat_Eq::dimen_x() const {
  return nx;
}

int Heat_Eq::dimen_y() const {
  return ny;
}

double ** Heat_Eq::get_grid() const{
  return T;
}
