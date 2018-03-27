#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include <string.h>

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
  Camera * cam = sp.getCamera();
  Group * g = sp.getGroup();
  float tmin = cam->getTMin();
  Image image(height, width);
  for (int i = 0; i < width; i++) {
	  for (int j = 0; j < height; j++) {
		  float x = float(i) / float(width-1) * 2 -1;
		  float y = float(j) / float(height - 1) * 2 - 1;
		  //create ray, find intersections.
		  Ray r = cam->generateRay(Vector2f(x, y));
		  Hit h = Hit();
		  bool intersected = g->intersect(r, h, tmin);
		  //write color
		  if (intersected) {
			  Vector3f intersection = r.getOrigin() + r.getDirection() * h.getT();
			  //ambient lighting based on texture/diffuse color
			  //cambient * ka + sum  L.N * clight * kd + L.R * clight * ks
			  Vector3f col = sp.getAmbientLight()*h.getMaterial()->getDiffuseColor(h);
			  
			  for (int k = 0; k < sp.getNumLights(); k++) {
				  //get light illumination
				  Vector3f lightCol;
				  Vector3f dirToLight;
				  float filler = 1;
				  //why does it even need a distance value when it's not used in Light.h
				  sp.getLight(k)->getIllumination(intersection, dirToLight, lightCol, filler);
				  Vector3f diff_spec = h.getMaterial()->Shade(r, h, dirToLight, lightCol);
				  col = col + diff_spec;

			  }
			  image.SetPixel(i, j, col);
			  
		  }
		  else {
			  image.SetPixel(i, j, sp.getBackgroundColor());
		  }
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

