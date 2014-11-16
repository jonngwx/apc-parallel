#include <stdlib.h>
#include <stdio.h>
#include "euler.h"
#include "utils.h"
#include "heat_eq_mpi.h"
#include <time.h>
#include <string.h>
#include "mpi.h"
#define PI 3.14159265358979

int main(int argc, char * argv[]){

  int size; int rank;
  clock_t time;
  time = clock();


  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (argc != 2){
    printf("Requires argument <nx> \n");
    return 0;
  }
  
  int nx = atoi(argv[1]);
  int ny = nx;

  if (nx < 2){
    printf("grid is too small. requires nx >= 3 for nontrivial solution.\n");
    MPI_Finalize();
    return 0;
  }
  if (nx %size !=0){
    printf("number of mpi tasks must be a factor of size \n");
    MPI_Finalize();
    return 0;
  }

  Model * model;
  Integrator *integrator;
  
  

  const double kappa = 1.;
  
  const double dx = PI/(nx-1);
  const double dt = dx*dx/4/kappa*.5; // stability condition.

  const double tmax = .5*PI*PI/kappa;
  double t = 0.;


  model = new Heat_Eq_Mpi(kappa,nx/size,ny,rank,size);

  integrator = new Euler(dt,*model);

  while (t<tmax){
    integrator->Step(t, (*model).get_grid());
    t = t+dt;
  }
  MPI_Status Stat;
  int tag = 1;
  if(rank==0){
    time = clock() - time;
    double **T = new double*[nx];
    for (int i = 0; i < nx; i++){
      T[i] = new double[ny]();
    }
    double **myT = (*model).get_grid();
    for (int i = 0; i < nx/size; i++){
      for (int j = 0; j < ny; j++){
	T[i][j] = myT[i+1][j];
      }
    }
    // collect data
    for (int i = 1; i < size; i++){
      MPI_Recv(T[i*nx/size],ny*nx/size, MPI_DOUBLE, i, tag, MPI_COMM_WORLD,&Stat);
    }
    char filename[80];
    strcpy(filename,"heat_out_mpi_");
    strcat(filename,argv[1]);
    double Tave;
    Tave = average(T, nx,ny);
    output_2d(T,nx,ny,filename);
    printf("%1.8f, %10.8f, %d\n",Tave, ((float)time)/CLOCKS_PER_SEC, rank);
    for (int i = 0; i < nx; i++){
      delete [] T[i];
    }    
    delete [] T;
  } else { // others send data and consolidate

    double **T = (*model).get_grid();
    MPI_Send((void*)T[1], ny*nx/size, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);

  }

  
  delete model;
  delete integrator;
  MPI_Finalize();

  return 0;
}
