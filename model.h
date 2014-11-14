#ifndef MODEL_H
#define MODEL_H


class Model {
 public:
  virtual ~Model(){};
  
  // RHS and boundary conditions
  virtual int rhs(double t, const double * const *x, double **fx) const =0;
  virtual void bcx(double t, const double * const *x, double **fx) const = 0;
  virtual void bcy(double t, const double * const *x, double **fx) const =0;
  virtual int dimen_x() const =0;
  virtual int dimen_y() const = 0;
  virtual double **get_grid() const =0;
};

#endif // MODEL_H
