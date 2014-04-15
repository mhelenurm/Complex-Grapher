#include "user_function.h"

double complex function(double complex x)
{
	double complex storage = x;
	unsigned int iterations = 10;
	for(unsigned int i = 0; i < iterations; i++)
	{
		storage = storage*storage+x;
	}
	return storage;
}
