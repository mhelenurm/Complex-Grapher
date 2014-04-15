#include "user_function.h"
#include <math.h>

double complex fxn2(double complex x, double complex c0)
{
  double complex c = fabs(creal(x)) + I*fabs(cimag(x));
  return c*c+c0;
}

double complex function(double complex x)
{
  double complex store = x;
  for(int i = 0; i < 8; i++)
  {
    store = fxn2(store, x);
  }
  return store;
}