#ifndef MHLIB_COLORS_H
#define MHLIB_COLORS_H

typedef unsigned int pixel;
typedef unsigned char sample;

#define MAX_SAMPLE (sample)255
#define MIN_SAMPLE (sample)0
#define SAMPLE_MASK 0xFF

extern sample sample_make(int component);
extern sample sample_makef(long double component);
extern pixel pixel_make(sample r, sample g, sample b, sample a);
extern pixel pixel_make_hsv(long double h, long double s, long double v);

extern sample pixel_alpha(pixel p);
extern sample pixel_red(pixel p);
extern sample pixel_green(pixel p);
extern sample pixel_blue(pixel p);

#endif
