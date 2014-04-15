#include "bitmap.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

void pixel_convert_to_16bpp(pixel p, unsigned char* loc)
{
        unsigned char r = (pixel_red(p)>>3)&0x1F;
        unsigned char g = (pixel_green(p)>>3)&0x1F;
        unsigned char b = (pixel_blue(p)>>3)&0x1F;
        unsigned short sum = (r<<10) + (g<<5) + (b);

	loc[0] = sum&0xFF;
        loc[1] = (sum>>8)&0xFF;
}

void pixel_convert_to_24bpp(pixel p, unsigned char* loc)
{
        loc[0] = pixel_blue(p);
        loc[1] = pixel_green(p);
        loc[2] = pixel_red(p);
}

void pixel_convert_to_32bpp(pixel p, unsigned char* loc)
{
        loc[0] = pixel_blue(p);
        loc[1] = pixel_green(p);
        loc[2] = pixel_red(p);
        loc[3] = pixel_alpha(p);
}

unsigned int bitmap_write(unsigned long width, unsigned long height, pixel* data, unsigned int bpp, char* filename)
{
	if(width==0 || height == 0 || data == 0 || bpp>2 || filename == 0)
	{
		printf("function bitmap_write has 1 or more invalid arguments");
		exit(1);
	}

	const unsigned int BITMAP_HEADER_SIZE_BYTES = 14;
	const unsigned int BITMAP_INFO_SIZE_BYTES = 40;

	unsigned int bits_per_pixel = 16;
	if(bpp == BITMAP_24BPP)
	{
		bits_per_pixel = 24;
	} else if(bpp == BITMAP_32BPP)
	{
		bits_per_pixel = 32;
	}

	const unsigned int BITMAP_ROW_SIZE_BYTES = 4*floor((bits_per_pixel * width + 31.0)/(32.0));
	const unsigned int BITMAP_DATA_SIZE_BYTES = BITMAP_ROW_SIZE_BYTES*height;

	unsigned char bitmap_header[BITMAP_HEADER_SIZE_BYTES] = {
                'B', 'M', //magic numbers
                0, 0, 0, 0, //size of file in bytes
                0, 0, //app data
                0, 0, //app data
                BITMAP_HEADER_SIZE_BYTES+BITMAP_INFO_SIZE_BYTES, 0, 0, 0 //start of data offset
        };

	unsigned short colorscheme = 16;
	if(bpp == BITMAP_24BPP)
	{
		colorscheme = 24;
	} else if(bpp == BITMAP_32BPP)
	{
		colorscheme = 32;
	}

	unsigned char bitmap_info[BITMAP_INFO_SIZE_BYTES] = {
                40,0,0,0, // info hd size
                0,0,0,0, // width
                0,0,0,0, // heigth
                1,0, // number color planes
                (unsigned char)(colorscheme&0xFF),(unsigned char)((colorscheme>>8)&0xFF), // bits per pixel
                0,0,0,0, // compression is none
                0,0,0,0, // image bits size
                0x13,0x0B,0,0, // horz resoluition in pixel / m
                0x13,0x0B,0,0, // vert resolutions (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
                0,0,0,0, // #colors in pallete
                0,0,0,0, // #important colors
        };

	const unsigned long sizeAll = BITMAP_DATA_SIZE_BYTES + BITMAP_HEADER_SIZE_BYTES + BITMAP_INFO_SIZE_BYTES;

	bitmap_header[ 2] = (unsigned char)( sizeAll    );
        bitmap_header[ 3] = (unsigned char)( sizeAll>> 8);
        bitmap_header[ 4] = (unsigned char)( sizeAll>>16);
        bitmap_header[ 5] = (unsigned char)( sizeAll>>24);

        bitmap_info[ 4] = (unsigned char)( width   );
        bitmap_info[ 5] = (unsigned char)( width>> 8);
        bitmap_info[ 6] = (unsigned char)( width>>16);
        bitmap_info[ 7] = (unsigned char)( width>>24);

        bitmap_info[ 8] = (unsigned char)( height    );
        bitmap_info[ 9] = (unsigned char)( height>> 8);
        bitmap_info[10] = (unsigned char)( height>>16);
        bitmap_info[11] = (unsigned char)( height>>24);

        bitmap_info[24] = (unsigned char)(BITMAP_DATA_SIZE_BYTES);
        bitmap_info[25] = (unsigned char)(BITMAP_DATA_SIZE_BYTES>>8);
        bitmap_info[26] = (unsigned char)(BITMAP_DATA_SIZE_BYTES>>16);
        bitmap_info[27] = (unsigned char)(BITMAP_DATA_SIZE_BYTES>>24);

	//now we translate the pixels into the new array
	unsigned char* alldata = (unsigned char*)calloc(1, BITMAP_DATA_SIZE_BYTES);
	for(int row  = 0; row < height; row++)
	{
		unsigned int index = BITMAP_ROW_SIZE_BYTES*row;
		for(int col = 0; col < width; col++)
		{
			if(bpp == BITMAP_16BPP)
			{
				pixel_convert_to_16bpp(data[row*width+col], &alldata[index]);
			} else if(bpp == BITMAP_24BPP)
			{
				pixel_convert_to_24bpp(data[row*width+col], &alldata[index]);
			} else if(bpp == BITMAP_32BPP)
			{
				pixel_convert_to_32bpp(data[row*width+col], &alldata[index]);
			}
			index+=bits_per_pixel/8;
		}
	}	

	FILE* bmpFile = fopen(filename, "w");
	fwrite(bitmap_header, 1, BITMAP_HEADER_SIZE_BYTES, bmpFile);
	fwrite(bitmap_info, 1, BITMAP_INFO_SIZE_BYTES, bmpFile);
	fwrite(alldata, 1, BITMAP_DATA_SIZE_BYTES, bmpFile);
	fclose(bmpFile);
	free(alldata);
	return 1;
}
