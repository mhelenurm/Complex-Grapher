#ifndef MHLIB_COLORS_H
#define MHLIB_COLORS_H

typedef unsigned int pixel;
typedef unsigned char sample;

#define MAX_SAMPLE (sample)255
#define MIN_SAMPLE (sample)0
#define SAMPLE_MASK 0xFF

sample sample_make(int component);
sample sample_makef(long double component);
pixel pixel_make(sample r, sample g, sample b, sample a);
pixel pixel_make_hsv(long double h, long double s, long double v);

sample pixel_alpha(pixel p);
sample pixel_red(pixel p);
sample pixel_green(pixel p);
sample pixel_blue(pixel p);

#endif
