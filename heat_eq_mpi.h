#ifndef HEAT_EQ_MPI_H
#define HEAT_EQ_MPI_H

#include "model.h"

class Heat_Eq_Mpi : public Model {
 public:
  Heat_Eq_Mpi(double kappa, int nx, int ny, int id, int ntasks);
  ~Heat_Eq_Mpi();
  int rhs(double t, const double * const *x, double **fx) const;
  void bcx(double t, const double *const *x, double **fx) const;
  void bcy(double t, const double *const *x, double **fx) const;
  void bcx_init() const;
  void bcy_init() const;
  int dimen_x() const;
  int dimen_y() const;
  double **get_grid() const;

  double **T;
  double *x;
  double *y;
  double *left; // to hold mpi data from the left
  double *right; // to hold mpi data from the right
  int tag;
  const int nx;
  const int ny;
  const int id; // MPI task ID
  const int ntasks; // total number of mpi tasks
 private:
  const double kappa;
};



#endif // HEAT_EQ_H
