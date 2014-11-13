#ifndef EULER_H_
#define EULER_H_

#include "integrator.h"
class Model;

class Euler : public Integrator {
 public:
  Euler(double dt, const Model &model);
  ~Euler();
  int Step(double t, double **x);
  double **fx;                          // temporary space to hold f(x,t)
 private:
  const int nx;                     // dimension of state x
  const int ny;
  const double dt_;                     // timestep
  const Model &model;                  // functor to evaluate f(x,t)
  
};

#endif  // EULER_H
