#ifndef UTILS_H
#define UTILS_H

double average(const double * const *T ,int nx, int ny);
void output_2d(const double * const * T, int nx, int ny, const char * filename);

// inline function to find the laplacian in 2-D
// T: the function to differentiate
// x: grid in x
// y: grid in y
// i,j: grid indices 
// here i could simply pass dx as a constant since we are using a uniform grid
// but this is not implemented
inline double nabla_squared(const double * const * T, const double *x, const double *y, int i, int j){
  return ((T[i-1][j] + T[i+1][j] - 2*T[i][j])/(x[i]-x[i-1])/(x[i]-x[i-1]) + (T[i][j-1] + T[i][j+1] - 2* T[i][j])/(y[j]-y[j-1])/(y[j]-y[j-1]));
  }

inline double d_squared_x(const double * const *T, const double *x, int i, int j){
  return (T[i-1][j] + T[i+1][j] - 2*T[i][j])/(x[i]-x[i-1])/(x[i] - x[i-1]);
}

inline double d_squared_y(const double * const *T, const double *y, int i, int j){
  return (T[i][j-1] + T[i][j+1] - 2* T[i][j])/(y[j]-y[j-1])/(y[j]-y[j-1]);
}

#endif
