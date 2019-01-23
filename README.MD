# Complex-Grapher

A c program that creates BMP images of complex functions.

The shell script ("runscript.exec") takes several arguments and compiles and runs the program with the arguments.

## Running The Program

./runscript.exec [imagewidth] [imageheight] [graphwidth] [graphheight] [graph center x] [graph center y] [output filename] [functionfilename].c


## Function Files
The function file is the file that does the complex transformations.
NAME: [functionfilename].c

## FUNCTIONFILE STRUCTURE:
***
#include "user_function.h"

double complex function(double complex x)
{
	//free to do whatever in here
	//return the transformed x
}
***

## Example Functions
./run_complexgrapher.exec 100 100 1 1 0 0 graph1.bmp function_x.c
./run_complexgrapher.exec 2560 1600 6 3.75 0 0 graph1.bmp function_prettyblue.c
./run_complexgrapher.exec 7680 4800 5 3.125 0.0 -0.4 graph_burning_ship.bmp function_burningship.c v
