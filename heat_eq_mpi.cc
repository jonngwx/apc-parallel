#include "heat_eq_mpi.h"
#include <math.h>
#include <stdio.h>
#include "mpi.h"
#include "utils.h"

Heat_Eq_Mpi::Heat_Eq_Mpi(double kappa, int nx0, int ny0, int id, int ntasks): nx(nx0+2), ny(ny0), id(id), ntasks(ntasks), kappa(kappa)
{
  // We will divide the domain along lines of constant x
  // so ny remains the same, but nx requires ghost cells
  x = new double[nx];
  y = new double[ny];
  T = new double*[nx];
  left = new double[ny]();
  right = new double[ny]();
  tag = 1;
  // domain is zero to pi in both dirs
  const double pi = 3.14159265358979;
  // dx = pi/ntasks/ncells
  const double dx = pi/(nx0*ntasks-1);
  const double offset = (nx-2)*dx*id -dx; // CHECK+ first and last cells overlap
  // dy remains the same
  const double dy = pi/(ny-1);
  for (int i = 0; i < nx; i++){
    x[i] = offset + i*dx;
    T[i] = new double[ny]();
  }//create x and y and grid;
  for (int j = 0; j < ny; j++){
    y[j] = 0 + j*dy;
  }
  bcx_init();
  bcy_init();
}

Heat_Eq_Mpi::~Heat_Eq_Mpi(){
  delete [] x;
  delete [] y;
  delete [] left;
  delete [] right;
  for (int i = 0; i < nx; i++){
    delete [] T[i];
  }
  delete [] T;
}

int Heat_Eq_Mpi::rhs(double t, const double * const *T, double ** fx) const{
  for (int i = 1; i < (nx - 1); i++){ // use 1,n-1 due to boundary terms
    for (int j = 1; j < (ny - 1); j++){
	fx[i][j] = kappa * nabla_squared(T,x,y,i,j);
      }
    }
  // boundary conditions calculate the new fx, not the new x. so they require the old T from the other side
  bcx(t, T, fx);
  bcy(t, T, fx);
    
  return 0;
}

// boundary conditions in x are periodic
// boundary conditions in y are not T(x,0) = cos^2 x
// T(x,pi) = sin^2 x
void Heat_Eq_Mpi::bcx(double t, const double * const *T, double ** fx) const{
  // create left and right;
  // order = even numbers send first, send to right then left
  // do mpi stuff, send and receive one row of data.
  // serial case
  if (ntasks == 1){
    for (int i =0; i < ny; i++){
      left[i] = T[nx-2][i];
    }
    for (int i =0; i < ny; i++){
      right[i] = T[1][i];
    }
  } else {
    MPI_Status Stat;
    if (id == 0) { // sending n-2 because n-1 T[1] of the next slice
      MPI_Send((void*)T[nx-2], ny, MPI_DOUBLE, id+1, tag, MPI_COMM_WORLD);
      MPI_Send((void*)T[1], ny, MPI_DOUBLE, ntasks-1, tag, MPI_COMM_WORLD);
      MPI_Recv(left,ny, MPI_DOUBLE, ntasks-1, tag, MPI_COMM_WORLD,&Stat);
      MPI_Recv(right,ny, MPI_DOUBLE, id+1, tag, MPI_COMM_WORLD,&Stat);
    }
    else if(id == ntasks-1){
      MPI_Recv(right,ny, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD,&Stat);
      MPI_Recv(left,ny, MPI_DOUBLE, id-1, tag, MPI_COMM_WORLD,&Stat);
      MPI_Send((void*)T[nx-2], ny, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD); // send right
      MPI_Send((void*)T[1], ny, MPI_DOUBLE, id-1, tag, MPI_COMM_WORLD); // send right
    } else { //receive left, send right
      MPI_Recv(left,ny, MPI_DOUBLE, id-1, tag, MPI_COMM_WORLD,&Stat);
      MPI_Send((void*)T[nx-2], ny, MPI_DOUBLE, id+1, tag, MPI_COMM_WORLD); // send right
      MPI_Recv(right,ny, MPI_DOUBLE, id+1, tag, MPI_COMM_WORLD,&Stat);
      MPI_Send((void*)T[1], ny, MPI_DOUBLE, id-1, tag, MPI_COMM_WORLD); // send right      
    }
  }

  // calculate the actual derivsx
  for(int j = 1; j < ny -1; j++){
    fx[0][j] = kappa * (d_squared_y(T,y,0,j) + (left[j] + T[1][j] -2 * T[0][j])/(x[1]-x[0])/(x[1]-x[0]));
    fx[nx-1][j] = kappa *(d_squared_y(T,y,nx-1,j) + (T[nx-2][j] - 2*T[nx-1][j] + right[j])/(x[1] -x[0])/(x[1]-x[0]));
  }
  
}

// boundary conditions at y are the same in mpi/non-mpi version
void Heat_Eq_Mpi::bcy(double t, const double * const *T, double ** fx) const{
  for (int i = 0; i < nx; i++){
    fx[i][0] = 0;//cos(x[i]) * cos(x[i]);
    fx[i][ny-1] =0;// sin(x[i]) * sin(x[i]);
  }
}

// empty, everything is zero initially
void Heat_Eq_Mpi::bcx_init() const{
  ;
  //  for (int j = 1; j < ny-1; j++){
  //T[0][j] = T[nx-1][j];
  //  }
}

void Heat_Eq_Mpi::bcy_init() const{
  for (int i = 0; i < nx; i++){
    T[i][0] = cos(x[i]) * cos(x[i]);
    T[i][ny-1] = sin(x[i]) * sin(x[i]);
  }
}

int Heat_Eq_Mpi::dimen_x() const {
  return nx;
}

int Heat_Eq_Mpi::dimen_y() const {
  return ny;
}

double ** Heat_Eq_Mpi::get_grid() const{
  return T;
}
