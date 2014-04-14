#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <complex.h>

#include "colors.c"

const long double K_PI = 3.1415926535897932384626433832795;
const long double K_E  = 2.7182818284590452353602874713527;

long double complex usr_fxn(long double complex x)
{
	return EPIC_MACRO;
}

long double complex function_1(long double complex x)
{
	//complex squared = complex_multiply(x, x);
	//complex denom = complex_add(complex_make(2, 2), squared);
	//complex num2 = complex_add(complex_make(-2, -1), x);
	//complex ggg = complex_multiply(complex_add(squared, complex_make(-1, 0)), complex_multiply(num2, num2));
	//return complex_divide(ggg, denom);
	long double complex x2 = x*x;
	const long double complex denomconst = 2+2I;
	const long double complex numconst1 = -2-I;
	const long double complex numconst2 = -1;
	long double complex num = x+numconst1;
	return (x2+numconst2)*num*num/(x2+denomconst);
}

pixel* create_graph(unsigned long imageWidth, unsigned long imageHeight, long double graph_width, long double graph_height, long double graph_cx, long double graph_cy, long double complex (*fxn)(long double complex), unsigned int is_verbose)
{
	pixel* data = (pixel*)malloc(sizeof(pixel)*imageWidth*imageHeight);
	if (ULONG_MAX/imageWidth < imageHeight)
	{
		printf("Image width (%lu) and height (%lu) are too large (their product is a ulong overflow).", imageWidth, imageHeight);
		exit(1);
	}
	long double complex graph_dx = graph_width/(long double)(imageWidth-1);
	long double complex graph_dy = graph_height/(long double)(imageHeight-1)*I; //MODIFIED

	//printf("DY: %f, %f\n", creal(graph_dy), cimag(graph_dy));

	long double complex graph_pos = (-graph_width/2.0+graph_cx) + (-graph_height/2.0+graph_cy)*I;//complex_make(-graph_width/(long double)2.0+graph_cx, -graph_height/(long double)2.0+graph_cy);
	//printf("POS: %f, %f\n", creal(graph_pos), cimag(graph_pos));
	unsigned int uses_output = (imageWidth*imageHeight > 1000*1000) && is_verbose;
	unsigned long cur_index = 0;
	unsigned long print_percent = 5;
	unsigned long percent_count = imageWidth*imageHeight/100;
	if(uses_output)
	{
		printf("\nRendering graph...\n");
	}

	for(unsigned long row = 0; row < imageHeight; row++)
	{
		for(unsigned long col = 0; col < imageWidth; col++)
		{
			//printf("%f, %f\n", creal(graph_pos), cimag(graph_pos));
			long double complex ggg = fxn(graph_pos);
		
			long double d = carg(ggg);//complex_argument(ggg);
			while(d < 0)
			{
				d+=K_PI*2.0;
			}
			while(d>K_PI*2.0)
			{
				d-=K_PI*2.0;
			}
			long double m = cabs(ggg);//complex_magnitude(ggg);
			long double sat = 1.0;
			long double val = 1.0;

			if(m < 1.0)
			{
				val = powl(m, .5);
			} else {
				long double log = logl(m);
				long double limit = 10.0;
				if(log > limit)
				{
					long double percent =  (log-limit)/limit;
					sat = 1.0-((percent<1.0)?percent:1.0);
					
					//val = 1.0-sat;
					//sat = m/10.0;
				}
			}

			sat = 1.0;
			val = 1.0;
			data[cur_index] = pixel_make_hsv(d/2.0/M_PI, sat, val);
			graph_pos += graph_dx; //MODIFIED
			cur_index++;
		}
		graph_pos = -graph_width/(long double)2.0+graph_cx + cimag(graph_pos)*I; //MODIFIED
		graph_pos += graph_dy; //MODIFIED
		if(uses_output && (cur_index%percent_count == 0 || cur_index==imageWidth*imageHeight))
		{
			unsigned long percent = (cur_index != imageWidth*imageHeight)?cur_index/percent_count:100;
			if(percent%print_percent == 0)
			{
				printf("%lu%c...\n", percent, '%');
			}
		}
	}
	if(uses_output)
	{
		printf("Done rendering graph.\n\n");
	}
	return data;
}

int main(int argc, char* argv[])
{
	unsigned char header[14] = {
		'B', 'M', //magic numbers
		0, 0, 0, 0, //size of file in bytes
		0, 0, //app data
		0, 0, //app data
		40+14, 0, 0, 0 //start of data offset
	};

	unsigned char info[40] = {
		40,0,0,0, // info hd size
		0,0,0,0, // width
		0,0,0,0, // heigth
		1,0, // number color planes
		32,0, // bits per pixel
		0,0,0,0, // compression is none
		0,0,0,0, // image bits size
		0x13,0x0B,0,0, // horz resoluition in pixel / m
		0x13,0x0B,0,0, // vert resolutions (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
		0,0,0,0, // #colors in pallete
		0,0,0,0, // #important colors
	};
	if(argc < 8)
	{
		printf("Too few arguments. Usage: ./a.out imw imh gw gy cx cy filename");
		exit(1);
	}
	unsigned int is_verbose = (argc == 9);
	unsigned long w = atoi(argv[1]);
	unsigned long h = atoi(argv[2]);
	if(w <= 0 || h <= 0)
	{
		printf("Width and height must be positive");
		exit(1);
	}

	long double graph_width = strtold(argv[3], NULL);
	long double graph_height = strtold(argv[4], NULL);
	long double cx = strtold(argv[5], NULL);
	long double cy = strtold(argv[6], NULL);

	if(graph_width <= 0 || graph_height <= 0)
	{
		printf("Graph width and height must be positive.");
		exit(1);
	}
	
	unsigned long sizeData = w*h*4;
	unsigned long sizeAll = sizeData+sizeof(header)+sizeof(info);

	header[ 2] = (unsigned char)( sizeAll    );
	header[ 3] = (unsigned char)( sizeAll>> 8);
	header[ 4] = (unsigned char)( sizeAll>>16);
	header[ 5] = (unsigned char)( sizeAll>>24);
	
	info[ 4] = (unsigned char)( w   );
	info[ 5] = (unsigned char)( w>> 8);
	info[ 6] = (unsigned char)( w>>16);
	info[ 7] = (unsigned char)( w>>24);
	
	info[ 8] = (unsigned char)( h    );
	info[ 9] = (unsigned char)( h>> 8);
	info[10] = (unsigned char)( h>>16);
	info[11] = (unsigned char)( h>>24);
	
	info[24] = (unsigned char)( sizeData    );
	info[25] = (unsigned char)( sizeData>> 8);
	info[26] = (unsigned char)( sizeData>>16);
	info[27] = (unsigned char)( sizeData>>24);
	
	pixel* pixels = create_graph(w, h, graph_width, graph_height, cx, cy, &usr_fxn, is_verbose);
	if(is_verbose)
	{
		printf("Writing file...\n");	
	}
	FILE* bmpFile = fopen(argv[7], "w");
	fwrite(header, 1, sizeof(header), bmpFile);
	fwrite(info, 1, sizeof(info), bmpFile);
	fwrite(pixels, 4, w*h, bmpFile);
	
	fclose(bmpFile);
	if(is_verbose)
	{
		printf("File written. Cleaning memory.\n\n");
	}
	free(pixels);
	return 0;
}
