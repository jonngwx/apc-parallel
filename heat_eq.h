#ifndef HEAT_EQ_H
#define HEAT_EQ_H

#include "model.h"

class Heat_Eq : public Model{
 public:
  Heat_Eq(double kappa, int nx, int ny);
  ~Heat_Eq();
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
  const int nx;
  const int ny;
 private:
  const double kappa;
};



#endif // HEAT_EQ_H
