#ifndef MODEL_H
#define MODEL_H


class Model {
 public:
  virtual ~Model(){};
  
  // RHS and boundary conditions
  virtual int rhs(double t, const double **x, double **fx) const;
  virtual void bcx(double t, const double **x, double **fx);
  virtual void bcy(double t, const double **x, double **fx);

};

#endif // MODEL_H
