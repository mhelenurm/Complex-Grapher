#include "user_function.h"
#include <math.h>

double complex fxn2(double complex x)
{
  return (x*x-I)/(2*x*x+2*I);
}

double complex function(double complex x)
{
  return fxn2(fxn2(fxn2(fxn2(x))));
}