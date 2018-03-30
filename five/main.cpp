#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include <string.h>
#include "RayTracer.h"
using namespace std;

float clampedDepth ( float depthInput, float depthMin , float depthMax);

#include "bitmap_image.hpp"
int main( int argc, char* argv[] )
{
  // Fill in your implementation here.

  // This loop loops over each of the input arguments.
  // argNum is initialized to 1 because the first
  // "argument" provided to the program is actually the
  // name of the executable (in our case, "a4").
  for( int argNum = 1; argNum < argc; ++argNum )
    {
      std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }
	
    
  // First, parse the scene using SceneParser.
  // Then loop over each pixel in the image, shooting a ray
  // through that pixel and finding its intersection with
  // the scene.  Write the color at the intersection to that
  // pixel in your output image.
  SceneParser sp = SceneParser(argv[2]);
  int width = stoi(argv[4]);
  int height = stoi(argv[5]);
  char * outFile = argv[7];
  int bounces = stoi(argv[10]);
  //always filter, jitter, bounce.
  Camera * cam = sp.getCamera();
  Group * g = sp.getGroup();
  float tmin = cam->getTMin();
  Image image(height, width);
  RayTracer rt = RayTracer(&sp,bounces);
  for (int i = 0; i < width; i++) {
	  for (int j = 0; j < height; j++) {
		  float x = float(i) / float(width - 1) * 2 - 1;
		  float y = float(j) / float(height - 1) * 2 - 1;

		  //create ray, find intersections.
		  Ray r = cam->generateRay(Vector2f(x, y));
		  Hit h = Hit();
		  Vector3f col = rt.traceRay(r, tmin, bounces, h);
		  //write color
		  image.SetPixel(i, j, col);

	  }
  }
  image.SaveImage(outFile);
 // std::getchar();
  /////TODO: below demonstrates how to use the provided Image class
  /////Should be removed when you start
  //Vector3f pixelColor (1.0f,0,0);
  ////width and height
  //Image image( 10 , 15 );
  //image.SetPixel( 5,5, pixelColor );
  //image.SaveImage("demo.bmp");
  //return 0;
}

