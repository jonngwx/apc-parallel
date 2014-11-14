#include "euler.h"
#include "model.h"

Euler::Euler(double dt, const Model &model)
    : nx(model.dimen_x()),
      ny(model.dimen_y()),
      dt_(dt),
      model(model) {
  fx = new double*[nx]; 
  for (int i = 0; i < nx; i++){
    fx[i] = new double[ny]();
  }
}

Euler::~Euler() {
  for (int i = 0; i < nx; i++){
    delete [] fx[i];
  }
  delete [] fx;
}

int Euler::Step(double t, double **x) {
  model.rhs(t, x, fx);
  for (int i = 0; i < nx; ++i) {
    for (int j = 0; j < ny; ++j){
      x[i][j] += dt_ * fx[i][j];
    }
  }
  return 0;
}
