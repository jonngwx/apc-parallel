#include "utils.h"
#include <stdio.h>

void output_2d(const double * const *T, int nx, int ny){
  for (int i=0; i < nx; i++){
    for (int j =0; j< ny; j++){
      printf("%2.8f ", T[i][j]);
    }
    printf("\n");
  }
}

double average(const double * const *T ,int nx, int ny){
  double Tave = 0;
  for (int i=0; i < nx; i++){
    for (int j = 0; j < ny; j++){
      Tave = Tave + T[i][j];
    }
  }
  Tave = Tave/nx/ny;
  return Tave;
}

//inline double nabla_squared(const double ** T, const double *x, const double *y, int i, int j){
//return ((T[i-1][j] + T[i+1][j] - 2*T[i][j])/(x[i]-x[i-1])/(x[i]-x[i-1]) + (T[i][j-1] + T[i][j+1] - 2* T[i][j])/(y[j]-y[j-1])/(y[j]-y[j-1]));
//  }
