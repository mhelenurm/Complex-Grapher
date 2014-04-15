#include "colors.h"
#include <math.h>

sample sample_make(int component)
{
	return (sample)(((unsigned int)component)&0xFF);
}

sample sample_makef(long double component)
{
	if(component < 0)
	{
		component = -component;
	}
	if(component >= 1.0)
	{
		return (sample)255;
	}
	return (sample)(component*256.0);
}

pixel pixel_make(sample r, sample g, sample b, sample a)
{
	return (pixel)((((unsigned int)a)<<24) + (((unsigned int)r)<<16) + (((unsigned int)g)<<8) + (unsigned int)b);
}

pixel pixel_make_hsv(long double h, long double s, long double v)
{
	long double angle = h*360.0;
	long double C = s * v;
	long double Hp = angle/60.0;
	
	long double X = C * (1-fabsl(fmodl((long double)Hp, (long double)2.0) - 1));
	
	long double r = 0, g = 0, b = 0;
	
	if(Hp<1.0)
	{
		r = C;
		g = X;
	} else if(Hp < 2.0)
	{
		r = X;
		g = C;
	} else if(Hp < 3.0)
	{
		g = C;
		b = X;
	} else if(Hp < 4.0)
	{
		g = X;
		b = C;
	} else if(Hp < 5.0)
	{
		r = X;
		b = C;
	} else if(Hp <= 6.0)
	{
		r = C;
		b = X;
	}
	long double m = v-C;
	return pixel_make(sample_makef(r+m), sample_makef(g+m), sample_makef(b+m), MAX_SAMPLE);
}

sample pixel_alpha(pixel p)
{
	return (sample)(p>>24) & SAMPLE_MASK;
}

sample pixel_red(pixel p)
{
	return (sample)(p >> 16) & SAMPLE_MASK;
}

sample pixel_green(pixel p)
{
	return (sample)(p>>8) & SAMPLE_MASK;
}

sample pixel_blue(pixel p)
{
	return (sample)p & SAMPLE_MASK;
}

#include <stdio.h>
void pixel_convert_to_16bpp(pixel p, unsigned char* loc)
{
	unsigned char r = (pixel_red(p)>>3)&0x1F;
	unsigned char g = (pixel_green(p)>>3)&0x1F;
	unsigned char b = (pixel_blue(p)>>3)&0x1F;
	unsigned short sum = (r<<10) + (g<<5) + (b);
	
	loc[1] = (sum>>8)&0xFF;
	loc[0] = sum&0xFF;	
}

void pixel_convert_to_24bpp(pixel p, unsigned char* loc)
{
	loc[2] = pixel_red(p);
	loc[1] = pixel_green(p);
	loc[0] = pixel_blue(p);
}

void pixel_convert_to_32bpp(pixel p, unsigned char* loc)
{
	loc[3] = pixel_alpha(p);
	loc[2] = pixel_red(p);
	loc[1] = pixel_green(p);
	loc[0] = pixel_blue(p);
}
