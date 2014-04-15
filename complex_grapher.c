#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <complex.h>

#include "colors.h"
#include "bitmap.h"
#include "user_function.h"

const double K_PI = 3.1415926535897932384626433832795;
const double K_E  = 2.7182818284590452353602874713527;

pixel* create_graph(unsigned long imageWidth, unsigned long imageHeight, double graph_width, double graph_height, double graph_cx, double graph_cy, unsigned int is_verbose)
{
  pixel* data = (pixel*)malloc(sizeof(pixel)*imageWidth*imageHeight);
  if (ULONG_MAX/imageWidth < imageHeight)
  {
    printf("Image width (%lu) and height (%lu) are too large (their product is a ulong overflow).", imageWidth, imageHeight);
    exit(1);
  }
  double complex graph_dx = graph_width/(long double)(imageWidth-1);
  double complex graph_dy = graph_height/(long double)(imageHeight-1)*I;
  
  double complex graph_pos = (-graph_width/2.0+graph_cx) + (-graph_height/2.0+graph_cy)*I;
  
  unsigned int uses_output = (imageWidth*imageHeight > 1000*1000) && is_verbose;
  unsigned long cur_index = 0;
  unsigned long print_percent = 5;
  unsigned long percent_count = imageWidth*imageHeight/100; //every percent_count iterations of the loop represents a percent
  
  double complex transformed; //this will represent the output of the complex function
	
  
  if(is_verbose) //if we're being verbose
  {
    printf("\nRendering graph...\n");
  }
  for(unsigned long row = 0; row < imageHeight; row++) //for every row
  {
    for(unsigned long col = 0; col < imageWidth; col++) //for every column
    {
      transformed = function(graph_pos);

      double arg = carg(transformed);
      if(arg<0)
      {
        arg+=M_PI*2.0;
      }
      arg/=M_PI*2.0;
      double mag = cabs(transformed);

      double sat = 1.0;
      double val = 1.0;

      if(mag < 1.0) //if the magnitude is less than one do something special
      {
        val = pow(mag, .5);
      } else {
        double logar = log(mag);
        double limit = 10.0;
        if(logar > limit)//if the magnitude is fuckin' huge we do this:
        {
          double percent =  (logar-limit)/limit/10.0;
          sat = 1.0-((percent<1.0)?percent:1.0);
        }
      }
      sat = 1.0;
      val = 1.0;
      data[cur_index] = pixel_make_hsv(arg, sat, val);

      graph_pos += graph_dx;
      cur_index++;
    }
    graph_pos = -graph_width/(long double)2.0+graph_cx + cimag(graph_pos)*I;
    graph_pos += graph_dy;
    
    if(uses_output && (cur_index%percent_count == 0 || cur_index==imageWidth*imageHeight)) //if we're printing percentages and this iteration is an exact percentage
    {
      unsigned long percent = (cur_index != imageWidth*imageHeight)?cur_index/percent_count:100; //the percent; 100 percent is hard to be exact about, so we have a special case for it
      if(percent%print_percent == 0) //we only wanna print every "print_percent" percentages
      {
        printf("%lu%c...\n", percent, '%'); //print the percentage
      }
    }
  }
  if(is_verbose) //if we're being verbose
  {
    printf("Done rendering graph.\n\n");
  }
  return data;
}

int main(int argc, char* argv[])
{
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
  
  double graph_width = atof(argv[3]);
  double graph_height = atof(argv[4]);
  double cx = atof(argv[5]);
  double cy = atof(argv[6]);
  
  if(graph_width <= 0 || graph_height <= 0)
  {
    printf("Graph width and height must be positive.");
    exit(1);
  }
	
  pixel* pixels = create_graph(w, h, graph_width, graph_height, cx, cy, is_verbose);
  
  if(is_verbose)
  {
    printf("Writing file...\n");
  }
  bitmap_write(w, h, pixels, BITMAP_32BPP, argv[7]);
  
  if(is_verbose)
  {
    printf("File written. Cleaning memory.\n\n");
  }
  free(pixels);
  return 0;
}
