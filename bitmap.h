#ifndef MHLIB_BITMAP_H
#define MHLIB_BITMAP_H
#include "colors.h"

#define BITMAP_16BPP 0
#define BITMAP_24BPP 1
#define BITMAP_32BPP 2

extern void pixel_convert_to_16bpp(pixel p, unsigned char* loc);
extern void pixel_convert_to_24bpp(pixel p, unsigned char* loc);
extern void pixel_convert_to_32bpp(pixel p, unsigned char* loc);

extern unsigned int bitmap_write(unsigned long width, unsigned long height, pixel* inputdata, unsigned int bpp, char* filename);

#endif
