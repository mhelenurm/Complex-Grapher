Complex-Grapher
===============

A c program that creates BMP images of complex functions.

The shell script ("runscript.exec") takes several arguments and compiles and runs the program with the arguments.

FORMAT:
./runscript.exec [imagewidth] [imageheight] [graphwidth] [graphheight] [graph center x] [graph center y] [output filename] "[function]".

The function must be in quotes. The variable is in terms of "x" and any library functions defined in C99's <complex.h> are fair game to use.

EXAMPLES:
./runscript.exec 2560 1600 6 3.75 0 0 graph1.bmp "(x^2)/(x-3-2*I)"
./runscript.exec 2560 1600 6 3.75 0 0 graph1.bmp "csin(1/x)"
